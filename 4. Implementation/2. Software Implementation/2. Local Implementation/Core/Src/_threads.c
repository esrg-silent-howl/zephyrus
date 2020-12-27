#include "_threads.h"
#include "_uss.h"
#include "_imu.h"
#include "main.h"
#include "adc.h"
#include "i2c.h"

/*!< Flag definitions */
#define F_TASKS_CREATED					0
#define F_SCHEDULER_STARTED			(F_TASKS_CREATED+1)

/*!< No error */
#define E_OK				(uint32_t)0
/*!< Error in message size */
#define E_MSG_SIZE	(uint32_t)(-1)
/*!< Error in number of sets */
#define E_SET_COUNT	(uint32_t)(-2)


/*!< System Threads Creation */
RTOS_TASK_STATIC(zMain, 1024, RP_BELOW_NORMAL, "zMain");
RTOS_TASK_STATIC(zGyroAccelerometerManager, 1024, RP_NORMAL, "zGyroAccelerometerManager");
RTOS_TASK_STATIC(zRFManager, 1024, RP_ABOVE_NORMAL, "zRFManager");
RTOS_TASK_STATIC(zInferenceManager, 1024, RP_HIGH, "zInferenceManager");
RTOS_TASK_STATIC(zUltrasonicManager, 1024, RP_REAL_TIME, "zUltrasonicManager");

/*!< System Mutexes Creation */
RTOS_MUTEX_STATIC(mState);
RTOS_MUTEX_STATIC(mReference);
RTOS_MUTEX_STATIC(mUSS);

/*!< System Semaphores Creation */
RTOS_SEMAPHORE_STATIC(semConfig, 3);
RTOS_SEMAPHORE_STATIC(semWorking, 4); 

/*!< System Notifications Creation */
RTOS_NOTIFICATION(nConfig, 0);
RTOS_NOTIFICATION(nConnected, 1);

/*!< System Queues Creation */
RTOS_QUEUE_STATIC(uss_queue, float, 2);

/*!< Flags Container*/
volatile uint32_t flags = 0;

/*!< Ultrasonic Sensor Containers */
USS_Sensor_t uss_front;
USS_Sensor_t uss_back;

/*!< System Common Timestamps Creation */
RTOS_TIMESTAMP(tsZero);

/*!< Current connection state */
RF_ConnectionState_t connection_state;

/*!< Utility functions declaration */
static void THREADS_incSemConfig(void);
static void THREADS_sleep(void);
static double THREADS_checkForLowBattery(void);


/*!< TIM2 IRQ Handler function. Included in stm32_f7xx_it.h and called from 
 *   TIM2_IRQHandler() to maintain consistency with the rest of the program. 
 *   This serves as an alternative to the HAL-generated handler which reads 
 *   the SR register before calling this one, clearing the interrupt flags */
void THREADS_tim2IRQHandler() {
	
	/*!< Read the SR register only once as the interrupt flags are cleared 
	 *   when this is done. */
	uint32_t sr = TIM2->SR;
	
	/*!< If it is a channel 3 (front sensor) interrupt */
	if (READ_BIT(sr, TIM_SR_CC3IF)) {
		
		/*!< In case of a rising edge, start counting */
		if (READ_BIT(USS_ECHO_FRONT_GPIO_Port->IDR, USS_ECHO_FRONT_Pin))
			USS_startCount(&uss_front, TIM2->CCR3);
		else
			USS_stopCount(&uss_front, TIM2->CCR3);
		
	/*!< Else if it is a channel 4 (back sensor) interrupt */
	} else if (READ_BIT(sr, TIM_SR_CC4IF)) {
		
		/*!< In case of a rising edge, start counting */
		if (READ_BIT(USS_ECHO_BACK_GPIO_Port->IDR, USS_ECHO_BACK_Pin))
			USS_startCount(&uss_back, TIM2->CCR4);
		else
			USS_stopCount(&uss_back, TIM2->CCR4);
	}
}

/*
void HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		i2c_rx_flag = 1;
}

void HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		i2c_tx_flag = 1;
}
*/
	
RTOS_TASK_FUN(zMain) {
 
#define BATTERY_LVL_MEAS_PERIOD		2000
#define POWER_LED_PERIOD_MS			2000
#define POWER_LED_DUTY_CYCLE		13
#define POWER_LED_PULSE_NORMAL		POWER_LED_PERIOD_MS
#define POWER_LED_PULSE_LOW_BATT	(POWER_LED_PERIOD_MS*POWER_LED_DUTY_CYCLE/100)
	
	/*!< Turn LED off (not making any assumptions about the current 
	 *   battery level) */
	WRITE_REG(TIM4->CCR2, POWER_LED_PULSE_LOW_BATT);
	
	/*!< Set the previously defined auto-reload value */
	WRITE_REG(TIM4->ARR, POWER_LED_PERIOD_MS);
	
	/*!< TIM4: Enable OC channel 2, main output and timer */
	HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_2);

	/*!< ADC1: Clear regular group conversion flag and overrun flag */
	CLEAR_BIT(ADC1->SR, ADC_FLAG_EOC | ADC_FLAG_OVR);
	
	/*!< ADC1: Enable end of conversion interrupt for regular group */
	SET_BIT(ADC1->CR1, ADC_IT_EOC | ADC_IT_OVR);
	
	/*!< ADC1: Enable ADC */
	SET_BIT(ADC1->CR2, ADC_CR2_ADON);
	
	/*!< Write the battery level measurement period in ms to the auto-reload 
	 *   register of TIM6 */
	WRITE_REG(TIM6->ARR, BATTERY_LVL_MEAS_PERIOD);
	
	/*!< Enable the TIM Update interrupt [DEBUG] */
	// SET_BIT(TIM6->DIER, TIM_DIER_UIE);
	
	/*!< TIM6: Start timer */
	SET_BIT(TIM6->CR1, TIM_CR1_CEN);

	while(1) {
		
		/*!< If a conversion is complete, interpret it */
		if (READ_BIT(ADC1->SR, ADC_FLAG_EOC)) {
			
			/* Clear EOC flag */
			CLEAR_BIT(ADC1->SR, ADC_FLAG_EOC);
			
			/*!< Read battery voltage and change led indicator accordingly */
			if (THREADS_checkForLowBattery())
				htim4.Instance->CCR2 = POWER_LED_PULSE_LOW_BATT;
			else
				htim4.Instance->CCR2 = POWER_LED_PULSE_NORMAL;
		}
		
		/*!< Wait for other threads to stop working */
		//while(RTOS_SEMAPHORE_GET_COUNT(semWorking) != 0){};
		
		/*!< Sleep until the next interruption */
		//THREADS_sleep(); 
	
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zGyroAccelerometerManager) {

#define Z_GYRO_ACC_MANAGER_PERIOD_MS	40
	
	RTOS_TIMESTAMP(timestamp);
	
	imu_t mpu6050;
	
	// while(!IMU_Init(&hi2c1));

	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	
	while(1) {
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
/*		
		IMU_dataRequest(&hi2c1);
		
		while (!i2c_rx_flag);
		i2c_rx_flag = 0;
		
		IMU_dataFetch(&mpu6050);
		
		double kx = mpu6050.KalmanAngleX;
		double ky = mpu6050.KalmanAngleY;
*/		
		/*!< Declare sleeping state */
		RTOS_SEMAPHORE_DEC(semWorking);
		
		/*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, Z_GYRO_ACC_MANAGER_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zRFManager) {

	RTOS_TIMESTAMP(timestamp);
	
	/*!< Wait for all configurations to be complete */
	RTOS_AWAIT(nConfig);
	
	RTOS_DELAY(200);
	
	/*!< Synchronization time stamp */
	RTOS_KEEP_TIMESTAMP(tsZero);
	
	/*!< Notify tasks of a successful connection */
	RTOS_NOTIFY(zGyroAccelerometerManager, nConnected);
	RTOS_NOTIFY(zInferenceManager, nConnected);
	RTOS_NOTIFY(zUltrasonicManager, nConnected);
	
	while(1) {	
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_INC(semWorking);

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    /*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, 20);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zInferenceManager) {

#define Z_INFERENCE_MANAGER_PERIOD_MS	40

	RTOS_TIMESTAMP(timestamp);
	float front_distance;
	float back_distance;
	
	volatile float front_distances[20];
	volatile float back_distances[20];
	volatile uint32_t index = 0;
	
	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	
	while(1) {
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
	
		RTOS_QUEUE_RECV(uss_queue, &front_distance);
		RTOS_QUEUE_RECV(uss_queue, &back_distance);

		front_distances[index%20] = front_distance;
		back_distances[index%20] = back_distance;
		
		if (++index == 20)
			while(1);
		
		
		/*!< Declare sleeping state */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    	/*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, Z_INFERENCE_MANAGER_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zUltrasonicManager) {

#define TIMER_CLK												108000000
#define Z_ULTRASONIC_MANAGER_PERIOD_MS	40
#define USS_CAPTURE_TIMEOUT_MS					25
	
 	RTOS_TIMESTAMP(timestamp);

	/*!< Initialize sensor containers */
	USS_init(&uss_front, 2.2, TIMER_CLK);
	USS_init(&uss_back, 2.2, TIMER_CLK);
	
	/*!< Stop TIM2 counter and reset it */
	CLEAR_BIT(TIM2->CR1, TIM_CR1_CEN);
	WRITE_REG(TIM2->CNT, 0);
	
	/*!< Enable Capture/Compare 3 and 4 interrupts */
	SET_BIT(TIM2->DIER, TIM_IT_CC3);
	SET_BIT(TIM2->DIER, TIM_IT_CC4);
	
	/*!< Enable the Input Capture channels 3 and 4 */
	SET_BIT(TIM2->CCER, TIM_CCER_CC3E);
	SET_BIT(TIM2->CCER, TIM_CCER_CC4E);
	
	/*!< Start- TIM2 counter */
	SET_BIT(TIM2->CR1, TIM_CR1_CEN);

	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);	
	
	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsZero, 5);
		
	while(1) {
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Pull trigger pin HIGH */
		USS_TRIGGER_GPIO_Port->BSRR = USS_TRIGGER_Pin;
		
		/*!< Enable the Output Capture channel 2 and write CNT + 15us 
		 *   to CCR2 register */
		SET_BIT(TIM2->CCER, TIM_CCER_CC2E);
		WRITE_REG(TIM2->CCR2, TIM2->CNT + 1620);
		
		/*!< Declare sleeping state */
		RTOS_SEMAPHORE_DEC(semWorking);
		
		/*!< Initial timing offset */
		RTOS_DELAY_UNTIL(timestamp, 25);
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Disable the Output Capture channel 2 */
		CLEAR_BIT(TIM2->CCER, TIM_CCER_CC2E);
			
		/*!< Calculate distance of the sensors with ready values and send them to 
		 *   the queue */
		USS_calculateDistance(&uss_front);
		USS_calculateDistance(&uss_back);
		RTOS_QUEUE_SEND_BACK(uss_queue, (void*)&uss_front.distance);
		RTOS_QUEUE_SEND_BACK(uss_queue, (void*)&uss_back.distance);
		
		/*!< Declare sleeping state */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    /*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, Z_ULTRASONIC_MANAGER_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}



void THREADS_create(void) {

	if (F_READ(F_TASKS_CREATED))
		return;

	RTOS_TASK_CREATE_STATIC(zMain);
	RTOS_TASK_CREATE_STATIC(zGyroAccelerometerManager);
	RTOS_TASK_CREATE_STATIC(zRFManager);
	RTOS_TASK_CREATE_STATIC(zInferenceManager);
	RTOS_TASK_CREATE_STATIC(zUltrasonicManager);

	RTOS_MUTEX_CREATE_STATIC(mState);
	RTOS_MUTEX_CREATE_STATIC(mReference);
	RTOS_MUTEX_CREATE_STATIC(mUSS);

	RTOS_SEMAPHORE_CREATE_STATIC(semConfig, 0);
	RTOS_SEMAPHORE_CREATE_STATIC(semWorking, 0);
	
	RTOS_QUEUE_CREATE_STATIC(uss_queue);
	
	F_SET(F_TASKS_CREATED);
}

void THREADS_startScheduler(void) {

	if (!F_READ(F_TASKS_CREATED) || F_READ(F_SCHEDULER_STARTED))
		return;

	RTOS_SCHEDULER_START();

	F_SET(F_SCHEDULER_STARTED);
}


static void THREADS_incSemConfig(void) {
	
	RTOS_SEMAPHORE_INC(semConfig);
	
	if (RTOS_SEMAPHORE_GET_COUNT(semConfig) == 3)
		RTOS_NOTIFY(zRFManager, nConfig);
}

static void THREADS_sleep(void){
	
	static uint32_t sleeps = 0;
	
	sleeps ++;
	
	/* Clear SLEEPDEEP bit of Cortex System Control Register */
	CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
	
	/* Request Wait For Interrupt */
	__WFI();
}


static double THREADS_checkForLowBattery(void) {
	
#define EVAL_LOW				1
#define EVAL_HIGH				0
#define VBATT_RSCALE		3
#define VBATT_MAX				8200
#define VBATT_MIN				6000
#define VBATT_EXCURSION	(VBATT_MAX-VBATT_MIN)
#define LOW_BATT_THRSLD	15
#define HYSTERESIS			2

	static int last_evaluation = EVAL_HIGH;	/*!< Evaluation from last iteration */
	int evaluation;			/*!< Evaluation to be made */
	int adc_value;			/*!< Raw ADC reading	*/
	int v_batt;					/*!< Battery voltage */
	int batt_lvl;				/*!< Calculated battery level */
	
	/*!< Fetch ADC conversion value */
	adc_value = READ_REG(ADC1->DR);

	/*!< Calculate real battery voltage */
	v_batt = adc_value * 3300 / 4096 * VBATT_RSCALE;
	
	/*!< Extrapolate 0-100 battery level assuming linear voltage drop */	
	batt_lvl = 100 * (v_batt - VBATT_MIN) / VBATT_EXCURSION;
		
	/*!< If last evaluation was LOW, go to HIGH only once the hysteresis margin 
	 *   has been overcome in the UP direction
	 *   If last evaluation was HIGH, go to LOW only once the hysteresis margin 
	 *   has been overcome in the DOWN direction */
	if (last_evaluation == EVAL_LOW)
		evaluation = batt_lvl < LOW_BATT_THRSLD + HYSTERESIS;	
	else
		evaluation = (batt_lvl <= LOW_BATT_THRSLD - HYSTERESIS);

	/*!< Keep calculated value for future reference */
	last_evaluation = evaluation;

	return evaluation;
}

