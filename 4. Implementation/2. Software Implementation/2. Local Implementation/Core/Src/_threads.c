#include "_threads.h"
#include "main.h"
#include "adc.h"
#include "tim.h"

/*!< Flag definitions */
#define F_TASKS_CREATED					0
#define F_SCHEDULER_STARTED			(F_TASKS_CREATED+1)

/*!< No error */
#define E_OK				(uint32_t)0
/*!< Error in message size */
#define E_MSG_SIZE	(uint32_t)(-1)
/*!< Error in number of sets */
#define E_SET_COUNT	(uint32_t)(-2)

/*!< System Threads Period */
#define PERIOD_GYRO_ACC_THREAD      xxxx
#define PERIOD_RF_THREAD            xxxx
#define PERIOD_INFERENCE_THREAD	    xxxx
#define PERIOD_ULTRASONIC_THREAD	xxxx

/*!< System Threads Creation */
RTOS_TASK_STATIC(zMain, 256, RP_REAL_TIME, "zMain");
RTOS_TASK_STATIC(zGyroAccelerometerManager, 256, RP_NORMAL, "zGyroAccelerometerManager");
RTOS_TASK_STATIC(zRFManager, 512, RP_ABOVE_NORMAL, "zRFManager");
RTOS_TASK_STATIC(zInferenceManager, 256, RP_HIGH, "zInferenceManager");
RTOS_TASK_STATIC(zUltrasonicManager, 256, RP_REAL_TIME, "zUltrasonicManager");

/*!< System Mutexes Creation */
RTOS_MUTEX_STATIC(mState);
RTOS_MUTEX_STATIC(mReference);
RTOS_MUTEX_STATIC(mUSS);

/*!< System Semaphores Creation */
RTOS_SEMAPHORE_STATIC(semConfig, 3);
RTOS_SEMAPHORE_STATIC(semWorking, 4); 

/*!< System Notifiations Creation */
RTOS_NOTIFICATION(nConfig, 0);
RTOS_NOTIFICATION(nConnected, 1);

/*!< Flags Container*/
uint32_t flags = 0;

/*!< System Timestamps Creation */
RTOS_TIMESTAMP(tsZero);

/*!< Current connection state */
RF_ConnectionState_t connection_state;

static void THREADS_incSemConfig(void) {
	
	RTOS_SEMAPHORE_INC(semConfig);
	
	if (RTOS_SEMAPHORE_GET_COUNT(semConfig) == 3)
		RTOS_NOTIFY(zRFManager, nConfig);
}

static void THREADS_sleep(void){}
static void THREADS_wakeUp(void){}
	
static double THREADS_checkForLowBattery(){
	
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
		
	/*!< If last evaluation was LOW, go to HIGH only once the hysteresis margin has been overcome in the UP direction
	 * If last evaluation was HIGH, go to LOW only once the hysteresis margin has been overcome in the DOWN direction */
	if (last_evaluation == EVAL_LOW)
		evaluation = batt_lvl < LOW_BATT_THRSLD + HYSTERESIS;	
	else
		evaluation = (batt_lvl <= LOW_BATT_THRSLD - HYSTERESIS);

	/*!< Keep calculated value for future reference */
	last_evaluation = evaluation;

	return evaluation;
}
	
volatile int bit;
RTOS_TASK_FUN(zMain) {
 
#define BATTERY_LVL_MEAS_PERIOD		2000
#define POWER_LED_PERIOD_MS				2000
#define POWER_LED_DUTY_CYCLE			13
#define POWER_LED_PULSE_NORMAL		POWER_LED_PERIOD_MS
#define POWER_LED_PULSE_LOW_BATT	(POWER_LED_PERIOD_MS*POWER_LED_DUTY_CYCLE/100)
	
	/*!< Turn LED off (not making any assumptions about the current battery level) */
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
	
	/*!< Write the battery level measurement period in ms to the auto-reload register of TIM6 */
	WRITE_REG(TIM6->ARR, BATTERY_LVL_MEAS_PERIOD);
	
	/*!< Enable the TIM Update interrupt [DEBUG] */
	// SET_BIT(TIM6->DIER, TIM_DIER_UIE);
	
	/*!< TIM6: Start timer */
	SET_BIT(TIM6->CR1, TIM_CR1_CEN);

	while(1) {
		
		/*!< If a conversion is complete, interpret it */
		if (READ_BIT(ADC1->SR, ADC_FLAG_EOC)) {
			
			CLEAR_BIT(ADC1->SR, ADC_FLAG_EOC);
			
			/*!< Read battery voltage and change led indicator accordingly */
			if (THREADS_checkForLowBattery())
				htim4.Instance->CCR2 = POWER_LED_PULSE_LOW_BATT;
			else
				htim4.Instance->CCR2 = POWER_LED_PULSE_NORMAL;
		}
		
		RTOS_DELAY(50);
		
		/*!< Wait for other threads to stop working */
		while(RTOS_SEMAPHORE_GET_COUNT(semWorking) != 0){};
		
		/*!< TODO: Sleep */
		THREADS_sleep(); 
			
		/*!< TODO: Wake up from sleep */
		THREADS_wakeUp();			
	}

	vTaskDelete(NULL);

}

RTOS_TASK_FUN(zGyroAccelerometerManager) {

	RTOS_TIMESTAMP(timestamp);

	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	
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

	vTaskDelete(NULL);
}

RTOS_TASK_FUN(zRFManager) {

	RTOS_TIMESTAMP(timestamp);
	
	/*!< Wait for all configurations to be complete */
	RTOS_AWAIT(nConfig);
	
	RTOS_DELAY(1000);
	
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

	vTaskDelete(NULL);
}

RTOS_TASK_FUN(zInferenceManager) {

	RTOS_TIMESTAMP(timestamp);

	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	
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

	vTaskDelete(NULL);
}

RTOS_TASK_FUN(zUltrasonicManager) {

 	RTOS_TIMESTAMP(timestamp);

	/*!< Signal another configuration complete */
	THREADS_incSemConfig();
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);	
	
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

	vTaskDelete(NULL);
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

	F_SET(F_TASKS_CREATED);
}

void THREADS_startScheduler(void) {

	if (!F_READ(F_TASKS_CREATED) || F_READ(F_SCHEDULER_STARTED))
		return;

	vTaskStartScheduler();

	F_SET(F_SCHEDULER_STARTED);
}