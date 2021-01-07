#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "_threads.h"
#include "_uss.h"
#include "_imu.h"
#include "_utils.h"
#include "_rf.h"

/*!< Flag definitions */
#define F_TASKS_CREATED					0
#define F_SCHEDULER_STARTED			(F_TASKS_CREATED+1)

/*!< No error */
#define E_OK				(uint32_t)0
/*!< Error in message size */
#define E_MSG_SIZE	(uint32_t)(-1)
/*!< Error in number of sets */
#define E_SET_COUNT	(uint32_t)(-2)

#define MASTER_CYCLE_PERIOD_MS	40
#define MASTER_INITIAL_DELAY_MS	10

/*!< System Threads Creation */
RTOS_TASK_STATIC(zMain, 1024, RP_BELOW_NORMAL, "zMain");
RTOS_TASK_STATIC(zIMUManager, 1024, RP_NORMAL, "zIMUManager");
RTOS_TASK_STATIC(zRFManager, 1024, RP_HIGH, "zRFManager");
RTOS_TASK_STATIC(zInferenceManager, 1024, RP_ABOVE_NORMAL, "zInferenceManager");
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
RTOS_NOTIFICATION(nIMURx, 2);
RTOS_NOTIFICATION(nIMUTx, 3);
static RTOS_NOTIFICATION(nRF, 4);

/*!< System Queues Creation */
RTOS_QUEUE_STATIC(qUSS, float, 2);
RTOS_QUEUE_STATIC(qIMU, float, 4);


/*!< Flags Container*/
volatile uint32_t flags = 0;

/*!< Ultrasonic Sensor Containers */
USS_Sensor_t uss_front;
USS_Sensor_t uss_back;

/*!< System Common Timestamps Creation */
RTOS_TIMESTAMP(tsConnect);

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
void THREADS_tim2IRQHandler(void) {
	
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

void THREADS_rfIRQHandler (void) {

	RTOS_NOTIFY_ISR(zRFManager, nRF);		
}

void HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		RTOS_NOTIFY_ISR(zIMUManager, nIMURx);
}

void HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		RTOS_NOTIFY_ISR(zIMUManager, nIMUTx);
}

	
RTOS_TASK_FUN(zMain) {
 
#define BATTERY_LVL_MEAS_PERIOD		2000
#define POWER_LED_PERIOD_MS				2000
#define POWER_LED_DUTY_CYCLE			13
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
	SET_BIT(TIM6->DIER, TIM_DIER_UIE);
	
	/*!< TIM6: Start timer */
	SET_BIT(TIM6->CR1, TIM_CR1_CEN);

	while(1) {
		
		/*!< If a conversion is complete, interpret it */
		if (READ_BIT(ADC1->SR, ADC_FLAG_EOC)) {
			
			/* Clear EOC flag */
			CLEAR_BIT(ADC1->SR, ADC_FLAG_EOC);
			
			/*!< Read battery voltage and change led indicator accordingly */
			if (THREADS_checkForLowBattery())
				WRITE_REG(TIM4->CCR2, POWER_LED_PULSE_LOW_BATT);
			else
				WRITE_REG(TIM4->CCR2, POWER_LED_PULSE_NORMAL);
		}
		
		/*!< Wait for other threads to stop working */
		//while(RTOS_SEMAPHORE_GET_COUNT(semWorking) != 0){};
		
		/*!< Sleep until the next interruption */
		//THREADS_sleep(); 
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zIMUManager) {

#define N_SAMPLES				4
#define N_SAMPLES_MASK	(N_SAMPLES - 1)
#define	IMU_SAMPLE_REQUEST_DELAY_MS	10
	
	
	RTOS_TIMESTAMP(tsIMUMan);
	
	imu_t mpu6050;
	volatile uint32_t samples = 0;
	float accel_x[N_SAMPLES];
	float accel_y[N_SAMPLES];
	float angle_z[N_SAMPLES];
	
	MODIFY_REG(I2C1->CR1, 0x000000FE, 0x00000001);
	//while(!IMU_Init(&hi2c1));
	
	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	tsIMUMan = tsConnect;

	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsIMUMan, MASTER_INITIAL_DELAY_MS);
	
	while(1) {

		/*!< Declare working state*/
		RTOS_SEMAPHORE_INC(semWorking);

		/*!< Request sensor data */
		IMU_dataRequest(&hi2c1);
		
		/*!< Await reception notification */
		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_AWAIT(nIMURx);
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Fetch received data */
		IMU_dataFetch(&mpu6050);
		
		/*!< Keep samples in the arrays */
		accel_x[samples] = mpu6050.Ax;
		accel_y[samples] = mpu6050.Ay;
		angle_z[samples] = mpu6050.Gz;
		samples = (samples + 1) & N_SAMPLES_MASK;
		
		/*!< When a sampling cycle ends, calculate averages and notify 
		 *   inference task*/
		if (samples == 0) {
			
			*accel_x = UTILS_calculateAverage(accel_x, N_SAMPLES);
			*accel_y = UTILS_calculateAverage(accel_y, N_SAMPLES);
			*angle_z = UTILS_calculateAverage(angle_z, N_SAMPLES);

			RTOS_QUEUE_SEND_BACK(qIMU, accel_x);
			RTOS_QUEUE_SEND_BACK(qIMU, accel_y);
			RTOS_QUEUE_SEND_BACK(qIMU, angle_z);
		}
		
		/*!< Sleep */
		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsIMUMan, IMU_SAMPLE_REQUEST_DELAY_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zRFManager) {

#define Z_RF_MANAGER_REQUEST_DELAY_MS	25
#define Z_RF_BROADCAST_PERIOD					500
#define Z_RF_HS_RESPONSE_TIMEOUT			5
#define Z_RF_PAYLOAD_SIZE							9
#define Z_RF_PERIOD_MS								35
#define Z_RF_QUEUE_TIMEOUT						5

#define Z_RF_MY_ADDRESS								{0x7E, 0x7E, 0x7E, 0x7E, 0x7E}
#define Z_RF_WB_ADDRESS								{0xE7, 0xE7, 0xE7, 0xE7, 0xE7}
#define Z_RF_HS_WB_CODE								{0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}
#define Z_RF_HS_MY_CODE								{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}
	
	volatile uint8_t my_address[5] = Z_RF_MY_ADDRESS;
	volatile uint8_t wb_address[5] = Z_RF_WB_ADDRESS;
	volatile const uint8_t my_code[Z_RF_PAYLOAD_SIZE] = Z_RF_HS_MY_CODE;
	volatile const uint8_t wb_code[Z_RF_PAYLOAD_SIZE] = Z_RF_HS_WB_CODE;
	uint8_t data_in[Z_RF_PAYLOAD_SIZE];
	uint8_t data_out[Z_RF_PAYLOAD_SIZE];

	RF_IRQ_t nrf_irq;
	RF_ConnectionState_t conn_state = NOT_CONNECTED;

	RTOS_TIMESTAMP(tsRFMan);

	/*!< Wait for all configurations to be complete */
	RTOS_AWAIT(nConfig);

/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	RF_Init(15, Z_RF_PAYLOAD_SIZE);
	
	/* Set 2MBps data rate and -18dBm output power */
	RF_SetRF(RF_DataRate_2M, RF_OutputPower_0dBm);

	
	/* Set my address, 5 bytes */
	RF_SetMyAddress((uint8_t*)my_address);
	
	/* Set TX address, 5 bytes */
	RF_SetTxAddress((uint8_t*)wb_address);
	
	/* Go back to RX mode */
	RF_PowerUpRx();
	
	volatile uint8_t st = RF_GetStatus();
	
	////////////////////////////// HANDSHAKE //////////////////////////////
	
	/*!< Loop while not connected */
	while(conn_state == NOT_CONNECTED) {
		
		int32_t it;

		/*!< Await message */
		RTOS_AWAIT(nRF);
		
		/*!< Read interrupt flags */
		RF_Read_Interrupts(&nrf_irq);
		
		/*!< If data was received, compare with the remote's code */
		if(nrf_irq.F.DataReady) {
			
			/*!< Read the code sent by the other device */
			RF_GetData(data_in);
			
			/*!< Check te received mesage or the code */
			for (it = Z_RF_PAYLOAD_SIZE-1; it >= 0; it--) {
				if (data_in[it] != wb_code[it])
					break;
			}
			
			/*!< Increment it to compare it with 0 down the line */
			it++;
			
			/*!< Give the remote time to respond */
			RTOS_DELAY(5);
			
			// If code correct
			if (it == 0) {
				
				/*!< Respond with the car's own code */
				RF_Transmit((uint8_t*)my_code);
				
				/*!< Give the remote time to respond */
				RTOS_DELAY(5);
				
				/*!< Await message */
				RTOS_AWAIT(nRF);
				
				/*!< Read interrupt flags */
				RF_Read_Interrupts(&nrf_irq);
				
				/*!< If transmission of the code was successful, aconnection is established */
				if(nrf_irq.F.DataSent) {
					break;
				}
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////
	
	/*!< Synchronization time stamp */
	RTOS_KEEP_TIMESTAMP(tsConnect);
	tsRFMan = tsConnect;
	
	/*!< Notify tasks of a successful connection */
	RTOS_NOTIFY(zIMUManager, nConnected);
	RTOS_NOTIFY(zInferenceManager, nConnected);
	RTOS_NOTIFY(zUltrasonicManager, nConnected);
	
	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsRFMan, MASTER_INITIAL_DELAY_MS + \
		Z_RF_MANAGER_REQUEST_DELAY_MS);
	
	while(1) {	

		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Somewhere */
		/*!< Timeout of RXmode */
		
		/*!< Sleep */
		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsRFMan, MASTER_CYCLE_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zInferenceManager) {

#define Z_INFERENCE_MANAGER_DELAY_MS	35
#define MOTOR_PWM_PERIOD_US						10000
	
	RTOS_TIMESTAMP(tsInfMan);
	
	float front_distance;
	float back_distance;

	float meas_accel_x;
	float meas_accel_y;
	float meas_angle_z;
	
	uint32_t motor_r_speed;
	uint32_t motor_l_speed;
	uint32_t motor_r_dir_inv;
	uint32_t motor_l_dir_inv;
	
	/*!< Stop TIM3 counter, reset it and set the appropriate autoreload value*/	
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	WRITE_REG(TIM3->CNT, 0);
	WRITE_REG(TIM3->ARR, MOTOR_PWM_PERIOD_US-1);
	
	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	tsInfMan = tsConnect;
	
	/*!< Enable the Input Capture channels 3 and 4 */
	SET_BIT(TIM3->CCER, TIM_CCER_CC3E);
	SET_BIT(TIM3->CCER, TIM_CCER_CC4E);
	
	/*!< Start TIM3 counter and reset it */	
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	
	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsInfMan, MASTER_INITIAL_DELAY_MS + \
		Z_INFERENCE_MANAGER_DELAY_MS);
	
	while(1) {
		
		/*!< Declare working state and keep current time stamp*/
		RTOS_SEMAPHORE_INC(semWorking);
	
		/*!< Get ultrasonic sensor readings */
		RTOS_QUEUE_RECV(qUSS, &front_distance);
		RTOS_QUEUE_RECV(qUSS, &back_distance);

		/*!< Get imu readings */
		RTOS_QUEUE_RECV(qIMU, &meas_accel_x);
		RTOS_QUEUE_RECV(qIMU, &meas_accel_y);
		RTOS_QUEUE_RECV(qIMU, &meas_angle_z);
		
		/*!< Sleep until the time to change the motor speed */
		RTOS_SEMAPHORE_DEC(semWorking);
		 
		/*!< Change motor direction and speed */
		motor_r_dir_inv = 0;
		motor_l_dir_inv = 0;
		SET_BIT(MOTOR_R_DIR_GPIO_Port->BSRR, MOTOR_R_DIR_Pin<<(16*motor_r_dir_inv));
		SET_BIT(MOTOR_L_DIR_GPIO_Port->BSRR, MOTOR_L_DIR_Pin<<(16*motor_l_dir_inv));
		motor_r_speed = 5000;
		motor_l_speed = 5000;
		WRITE_REG(TIM3->CCR3, motor_r_speed);
		WRITE_REG(TIM3->CCR4, motor_l_speed);
		
		/*!< Sleep */
		RTOS_SEMAPHORE_DEC(semWorking);  
		RTOS_DELAY_UNTIL(tsInfMan, MASTER_CYCLE_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zUltrasonicManager) {

#define TIMER_CLK												108000000
#define Z_ULTRASONIC_MANAGER_DELAY_MS		5
#define USS_CAPTURE_TIMEOUT_MS					30
	
	RTOS_TIMESTAMP(tsUSSMan);

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
	tsUSSMan = tsConnect;
	
	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsUSSMan, MASTER_INITIAL_DELAY_MS + \
		Z_ULTRASONIC_MANAGER_DELAY_MS);

	while(1) {
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Pull trigger pin HIGH */
		USS_TRIGGER_GPIO_Port->BSRR = USS_TRIGGER_Pin;
		
		/*!< Enable the Output Capture channel 2 and write CNT + 15us 
		 *   to CCR2 register */
		SET_BIT(TIM2->CCER, TIM_CCER_CC2E);
		WRITE_REG(TIM2->CCR2, TIM2->CNT + 1620);
		
		/*!< Sleep */
		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsUSSMan, USS_CAPTURE_TIMEOUT_MS);
		
		/*!< Declare working state */
		RTOS_SEMAPHORE_INC(semWorking);
		
		/*!< Disable the Output Capture channel 2 */
		CLEAR_BIT(TIM2->CCER, TIM_CCER_CC2E);
			
		/*!< Calculate distance of the sensors with ready values and send them to 
		 *   the queue */
		USS_calculateDistance(&uss_front);
		USS_calculateDistance(&uss_back);
		RTOS_QUEUE_SEND_BACK(qUSS, (void*)&uss_front.distance);
		RTOS_QUEUE_SEND_BACK(qUSS, (void*)&uss_back.distance);
		
		/*!< Sleep */
		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsUSSMan, MASTER_CYCLE_PERIOD_MS - USS_CAPTURE_TIMEOUT_MS);
	}

	RTOS_TASK_DELETE();
}



void THREADS_create(void) {

	if (F_READ(F_TASKS_CREATED))
		return;

	RTOS_TASK_CREATE_STATIC(zMain);
	RTOS_TASK_CREATE_STATIC(zIMUManager);
	RTOS_TASK_CREATE_STATIC(zRFManager);
	RTOS_TASK_CREATE_STATIC(zInferenceManager);
	RTOS_TASK_CREATE_STATIC(zUltrasonicManager);

	RTOS_MUTEX_CREATE_STATIC(mState);
	RTOS_MUTEX_CREATE_STATIC(mReference);
	RTOS_MUTEX_CREATE_STATIC(mUSS);

	RTOS_SEMAPHORE_CREATE_STATIC(semConfig, 0);
	RTOS_SEMAPHORE_CREATE_STATIC(semWorking, 0);
	
	RTOS_QUEUE_CREATE_STATIC(qUSS);
	RTOS_QUEUE_CREATE_STATIC(qIMU);
	
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
	
	sleeps++;
	
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

	static uint32_t last_evaluation = EVAL_HIGH;	/*!< Evaluation from last iteration */
	uint32_t evaluation;			/*!< Evaluation to be made */
	int32_t adc_value;			/*!< Raw ADC reading	*/
	int32_t v_batt;					/*!< Battery voltage */
	int32_t batt_lvl;				/*!< Calculated battery level */
	
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
