#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "_threads.h"
#include "_imu.h"
#include "_utils.h"

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
static RTOS_TASK_STATIC(zMain, 1024, RP_REAL_TIME/*RP_BELOW_NORMAL*/, "zMain");
static RTOS_TASK_STATIC(zIMUManager, 1024, RP_NORMAL, "zIMUManager");
static RTOS_TASK_STATIC(zRFManager, 1024, RP_HIGH, "zRFManager");

/*!< System Semaphores Creation */
static RTOS_SEMAPHORE_STATIC(semJoin, 2); 

/*!< System Notifications Creation */
static RTOS_NOTIFICATION(nConfig, 0);
static RTOS_NOTIFICATION(nConnected, 1);
static RTOS_NOTIFICATION(nIMURx, 2);
static RTOS_NOTIFICATION(nIMUTx, 3);

/*!< System Queues Creation */
static RTOS_QUEUE_STATIC(qIMU, float, 4);

/*!< Flags Container*/
static volatile uint32_t flags = 0;

/*!< System Common Timestamps Creation */
RTOS_TIMESTAMP(tsConnect);

/*!< Current connection state */
RF_ConnectionState_t connection_state;

/*!< Utility functions declaration */
static void THREADS_sleep(void);
static double THREADS_checkForLowBattery(void);


void HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		RTOS_NOTIFY_ISR(zIMUManager, nIMURx);
}

void HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1)
		RTOS_NOTIFY_ISR(zIMUManager, nIMUTx);
}

	
RTOS_TASK_FUN(zMain) {
 
#define BATTERY_LVL_MEAS_PERIOD		1000
#define POWER_LED_PERIOD_MS				1000
#define POWER_LED_DUTY_CYCLE			15
#define POWER_LED_PULSE_NORMAL		POWER_LED_PERIOD_MS
#define POWER_LED_PULSE_LOW_BATT	(POWER_LED_PERIOD_MS*POWER_LED_DUTY_CYCLE/100)
#define MAIN_SAMPLE_CHECK_PERIOD	1000
	
	RTOS_TIMESTAMP(tsMain);
	
	/*!< Stop LED_PWR and ADC trigger timers and reset counters */
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	CLEAR_BIT(TIM15->CR1, TIM_CR1_CEN);
	WRITE_REG(TIM3->CNT, 0);
	WRITE_REG(TIM15->CNT, 0);
	
	/*!< Turn LED off (not making any assumptions about the current 
	 *   battery level) */
	WRITE_REG(TIM3->CCR1, 0);
	
	/*!< Enable OC channel 1 and main output on PWR_LED timer */
	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);
	SET_BIT(TIM3->BDTR, TIM_BDTR_MOE);
	
		/*!< Set the previously defined auto-reload value */
	WRITE_REG(TIM3->ARR, POWER_LED_PERIOD_MS);
	
	/*!< Write the battery level measurement period in ms to the auto-reload 
	 *   register of TIM15 */
	WRITE_REG(TIM15->ARR, BATTERY_LVL_MEAS_PERIOD);
	
	/*!< Start ADC conversions */
	// HAL_ADC_Start(&hadc);

	/*!< ADC1: Clear regular group conversion flag and overrun flag */
	CLEAR_BIT(ADC1->ISR, ADC_FLAG_EOC | ADC_FLAG_OVR);
	
	/*!< ADC1: Enable end of conversion interrupt for regular group */
	SET_BIT(ADC1->CR, ADC_IT_EOC | ADC_IT_OVR);
	
	/*!< ADC1: Enable ADC */
	SET_BIT(ADC1->CR, ADC_CR_ADEN | ADC_CR_ADSTART);
	
	/*!< Enable the TIM Update interrupt [DEBUG] */
	// SET_BIT(TIM15->DIER, TIM_DIER_UIE);
	
	/*!< Start LED_PWR and ADC trigger timers */
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	SET_BIT(TIM15->CR1, TIM_CR1_CEN);
	
	while(1) {
		
		/*!< If a conversion is complete, interpret it */
		if (READ_BIT(ADC1->ISR, ADC_FLAG_EOC)) {
			
			/* Clear EOC flag */
			CLEAR_BIT(ADC1->ISR, ADC_FLAG_EOC);
			
			/*!< Read battery voltage and change led indicator accordingly */
			if (THREADS_checkForLowBattery())
				WRITE_REG(TIM3->CCR1, POWER_LED_PULSE_LOW_BATT);
			else
				WRITE_REG(TIM3->CCR1, POWER_LED_PULSE_NORMAL);
		}
		
		RTOS_DELAY_UNTIL(tsMain, MAIN_SAMPLE_CHECK_PERIOD);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zIMUManager) {

#define N_SAMPLES				4
#define N_SAMPELS_MASK	(N_SAMPLES - 1)
#define	IMU_SAMPLE_REQUEST_DELAY_MS	10
	
	RTOS_TIMESTAMP(tsIMUMan);
//	imu_t mpu6050;
//	volatile uint32_t samples = 0;
//	float accel_x[N_SAMPLES];
//	float accel_y[N_SAMPLES];
//	float angle_z[N_SAMPLES];
//	
//	MODIFY_REG(I2C1->CR1, 0x000000FE, 0x00000001);
//	while(!IMU_Init(&hi2c1));
//	
//	/*!< Signal another configuration complete */
//	THREADS_incSemConfig();
//	
//	/*!< Await the establishment of a connection to continue */
//	RTOS_AWAIT(nConnected);
//	tsIMUMan = tsConnect;

//	/*!< Initial timing offset */
//	RTOS_DELAY_UNTIL(tsIMUMan, MASTER_INITIAL_DELAY_MS);
	
	while(1) {

//		/*!< Declare working state*/
//		RTOS_SEMAPHORE_INC(semWorking);

//		/*!< Request sensor data */
//		IMU_dataRequest(&hi2c1);
//		
//		/*!< Await reception notification */
//		RTOS_SEMAPHORE_DEC(semWorking);
//		RTOS_AWAIT(nIMURx);
//		RTOS_SEMAPHORE_INC(semWorking);
//		
//		/*!< Fetch received data */
//		IMU_dataFetch(&mpu6050);
//		
//		/*!< Keep samples in the arrays */
//		accel_x[samples] = mpu6050.Ax;
//		accel_y[samples] = mpu6050.Ay;
//		angle_z[samples] = mpu6050.Gz;
//		samples = (samples + 1) & N_SAMPELS_MASK;
//		
//		/*!< When a sampling cycle ends, calculate averages and notify 
//		 *   inference task*/
//		if (samples == 0) {
//			
//			*accel_x = UTILS_calculateAverage(accel_x, N_SAMPLES);
//			*accel_y = UTILS_calculateAverage(accel_y, N_SAMPLES);
//			*angle_z = UTILS_calculateAverage(angle_z, N_SAMPLES);

//			RTOS_QUEUE_SEND_BACK(qIMU, accel_x);
//			RTOS_QUEUE_SEND_BACK(qIMU, accel_y);
//			RTOS_QUEUE_SEND_BACK(qIMU, angle_z);
//		}
//		
//		/*!< Sleep */
//		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsIMUMan, IMU_SAMPLE_REQUEST_DELAY_MS);
	}

	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zRFManager) {

//#define Z_RF_MANAGER_REQUEST_DELAY_MS	25

	RTOS_TIMESTAMP(tsRFMan);

//	/*!< Wait for all configurations to be complete */
//	RTOS_AWAIT(nConfig);
//	
//	RTOS_DELAY(200);
//	
//	/*!< Synchronization time stamp */
//	RTOS_KEEP_TIMESTAMP(tsConnect);
//	tsRFMan = tsConnect;
//	
//	/*!< Notify tasks of a successful connection */
//	RTOS_NOTIFY(zIMUManager, nConnected);
//	RTOS_NOTIFY(zInferenceManager, nConnected);
//	RTOS_NOTIFY(zUltrasonicManager, nConnected);
//	
//	/*!< Initial timing offset */
//	RTOS_DELAY_UNTIL(tsRFMan, MASTER_INITIAL_DELAY_MS + \
//		Z_RF_MANAGER_REQUEST_DELAY_MS);
	
	while(1) {	

//		/*!< Declare working state */
//		RTOS_SEMAPHORE_INC(semWorking);
//		
//		/*!< Somewhere */
//		/*!< Timeout of RXmode */
//		
//		/*!< Sleep */
//		RTOS_SEMAPHORE_DEC(semWorking);
		RTOS_DELAY_UNTIL(tsRFMan, MASTER_CYCLE_PERIOD_MS);
	}

	RTOS_TASK_DELETE();
}


void THREADS_create(void) {

	if (F_READ(F_TASKS_CREATED))
		return;

	RTOS_TASK_CREATE_STATIC(zMain);
	RTOS_TASK_CREATE_STATIC(zIMUManager);
	RTOS_TASK_CREATE_STATIC(zRFManager);
	
	RTOS_QUEUE_CREATE_STATIC(qIMU);
	
	F_SET(F_TASKS_CREATED);
}

void THREADS_startScheduler(void) {

	if (!F_READ(F_TASKS_CREATED) || F_READ(F_SCHEDULER_STARTED))
		return;

	RTOS_SCHEDULER_START();

	F_SET(F_SCHEDULER_STARTED);
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
#define VBATT_MAX				3300
#define VBATT_MIN				2700
#define VBATT_EXCURSION	(VBATT_MAX-VBATT_MIN)
#define LOW_BATT_THRSLD	15
#define HYSTERESIS			2
#define VREFINT_CAL_ADDR ((uint16_t*)((uint32_t)0x1FFFF7BA))
	
	static uint32_t last_evaluation = EVAL_HIGH;	/*!< Evaluation from last iteration */
	volatile uint32_t evaluation;			/*!< Evaluation to be made */
	volatile int32_t adc_value;			/*!< Raw ADC reading	*/
	volatile int32_t v_batt;					/*!< Battery voltage */
	volatile int32_t batt_lvl;				/*!< Calculated battery level */
	
	/*!< Fetch ADC conversion value */
	adc_value = READ_REG(ADC1->DR);
	
	/*!< Calculate real battery voltage
	 *   VDDA = 3.3 V x VREFIN_CAL / VREFINT_DATA */
	v_batt = 3300*(*VREFINT_CAL_ADDR)/adc_value;
	
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
