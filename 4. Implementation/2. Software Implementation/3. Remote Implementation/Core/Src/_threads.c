#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "_threads.h"
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
#define MASTER_INITIAL_DELAY_MS	5

/*!< System Threads Creation */
static RTOS_TASK_STATIC(zMain, 1024, RP_BELOW_NORMAL, "zMain");
static RTOS_TASK_STATIC(zIMUManager, 1024, RP_NORMAL, "zIMUManager");
static RTOS_TASK_STATIC(zRFManager, 1024, RP_HIGH, "zRFManager");

/*!< System Semaphores Creation */
static RTOS_SEMAPHORE_STATIC(semTerminate, 4); 

/*!< System Notifications Creation */
static RTOS_NOTIFICATION(nConfig, 0);
static RTOS_NOTIFICATION(nConnected, 1);
static RTOS_NOTIFICATION(nIMURx, 2);
static RTOS_NOTIFICATION(nIMUTx, 3);
static RTOS_NOTIFICATION(nRF, 4);

/*!< System Queues Creation */
static RTOS_QUEUE_STATIC(qIMU, float, 2);

/*!< Flags Container */
static volatile uint32_t flags = 0;

/*!< System Common Timestamps Creation */
RTOS_TIMESTAMP(tsConnect);

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

void THREADS_rfIRQHandler (void) {

	RTOS_NOTIFY_ISR(zRFManager, nRF);		
}

void THREADS_shutdownIRQHandler (void) {
	
	/*!< Avoid another interruption action on further button presses */
	if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == 0) 
		RTOS_SEMAPHORE_INC_ISR(semTerminate);		
}

RTOS_TASK_FUN(zMain) {
 
#define POWER_LED_DUTY_CYCLE			15
#define POWER_LED_PULSE_NORMAL		BATTERY_SAMPLE_PERIOD
#define POWER_LED_PULSE_LOW_BATT	\
	(((BATTERY_SAMPLE_PERIOD + 1)*POWER_LED_DUTY_CYCLE/100)-1)
#define Z_MAIN_PERIOD_MS					200
	
	RTOS_TIMESTAMP(tsMain);
	
	/*!< TIM2: Stop power timer and set counter to the value of CCR1 */
	CLEAR_BIT(TIM2->CR1, TIM_CR1_CEN);
	WRITE_REG(TIM2->CNT, BATTERY_SAMPLE_PERIOD);
	
	/*!< TIM2: Turn LED on to indicate power up */
	WRITE_REG(TIM2->CCR2, BATTERY_SAMPLE_PERIOD);
	
	/*!< Enable OC channel 1 and main output on the power timer */
	SET_BIT(TIM2->CCER, TIM_CCER_CC2E);
	SET_BIT(TIM2->BDTR, TIM_BDTR_MOE);
	
	/*!< TIM2: Start */
	SET_BIT(TIM2->CR1, TIM_CR1_CEN);
	
	/*!< ADC1: Clear regular group conversion flag and overrun flag */
	CLEAR_BIT(ADC1->ISR, ADC_FLAG_EOC | ADC_FLAG_OVR);
	
	/*!< Calibrate the ADC */
	WRITE_REG(ADC1->CR, 0);
	SET_BIT(ADC1->CR, ADC_CR_ADCAL);
	
	/*!< ADC1: Enable end of conversion interrupt for regular group */
	SET_BIT(ADC1->CR, ADC_IT_EOC | ADC_IT_OVR);
	
	/*!< ADC1: Enable ADC */
	SET_BIT(ADC1->CR, ADC_CR_ADEN | ADC_CR_ADSTART);
	
	while(1) {
		
		/*!< If a conversion is complete, interpret it */
		if (READ_BIT(ADC1->ISR, ADC_FLAG_EOC)) {
			
			/*!< [DEBUG] Toggle pin to indicate entrance in battery calculation routine */
			// HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
			
			/*!< Clear EOC flag */
			CLEAR_BIT(ADC1->ISR, ADC_FLAG_EOC);
			
			/*!< Read battery voltage and change led indicator accordingly */
			if (THREADS_checkForLowBattery())
				WRITE_REG(TIM2->CCR2, POWER_LED_PULSE_LOW_BATT);
			else
				WRITE_REG(TIM2->CCR2, POWER_LED_PULSE_NORMAL);
		}
		
		/*!< Sleep */
		RTOS_DELAY_UNTIL(tsMain, Z_MAIN_PERIOD_MS);

		if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == 3)
			goto cleanup;
	}
	
	cleanup:
	RTOS_SEMAPHORE_INC(semTerminate);
	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zIMUManager) {

#define N_SAMPLES										1
#define N_SAMPLES_MASK							(N_SAMPLES - 1)
#define	Z_IMU_PERIOD_MS							10
#define IMU_QUEUE_TIMEOUT						5
	
	RTOS_TIMESTAMP(tsIMUMan);
	imu_t mpu6050;
	
	volatile uint32_t samples = 0;
	float angle_x[N_SAMPLES];
	float angle_y[N_SAMPLES];
	
	MODIFY_REG(I2C1->CR1, 0x000000FE, 0x00000001);
	//while(!IMU_Init(&hi2c1));
	
	/*!< Enter low power mode */
	IMU_enterLowPowerMode(&hi2c1);
	
	/*!< Signal configuration complete */
	RTOS_NOTIFY(zRFManager, nConfig);
	
	/*!< Await the establishment of a connection to continue */
	RTOS_AWAIT(nConnected);
	tsIMUMan = tsConnect;

	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsIMUMan, MASTER_INITIAL_DELAY_MS);
	
	while(1) {
		
		/*!< Exit low power mode */
		IMU_exitLowPowerMode(&hi2c1);
		
		/*!< Request sensor data */
		IMU_dataRequest(&hi2c1);
		
		/*!< Await reception notification */
		RTOS_AWAIT(nIMURx);
		
		/*!< Fetch received data */
		IMU_dataFetch(&mpu6050);
		
		/*!< Enter low power mode */
		IMU_enterLowPowerMode(&hi2c1);
		
		/*!< Keep samples in the arrays */
		angle_x[samples] = mpu6050.Gx;
		angle_y[samples] = mpu6050.Gy;
	
#if (NSAMPLES > 1)
		samples = (samples + 1) & N_SAMPLES_MASK;
		/*!< When a sampling cycle ends, calculate averages and notify 
		 *   inference task*/
		if (samples == 0) {
#endif

		/*!< Calculate averages of the oversampled values */
		*angle_x = UTILS_calculateAverage(angle_x, N_SAMPLES);
		*angle_y = UTILS_calculateAverage(angle_y, N_SAMPLES);
	
		/*!< Queue the angle values for zRFManager */
		/*!< Timeout is meant to keep the task responsive in the case that 
		zRFManager did not yet retrieve the previous items , such as when it has
		reponded to the terminate signal */
		RTOS_QUEUE_SEND_BACK_TIMEOUT(qIMU, angle_x, IMU_QUEUE_TIMEOUT);
		RTOS_QUEUE_SEND_BACK_TIMEOUT(qIMU, angle_y, IMU_QUEUE_TIMEOUT);

#if (NSAMPLES > 1)	
		}
#endif

		/*!< Sleep */
		RTOS_DELAY_UNTIL(tsIMUMan, Z_IMU_PERIOD_MS);
		
		if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == 2)
			goto cleanup;
	}
	
	cleanup:
	RTOS_SEMAPHORE_INC(semTerminate);
	RTOS_TASK_DELETE();
}

RTOS_TASK_FUN(zRFManager) {

#define Z_RF_BROADCAST_PERIOD					500
#define Z_RF_HS_RESPONSE_TIMEOUT			50
#define Z_RF_PAYLOAD_SIZE							9
#define Z_RF_PERIOD_MS								35
#define Z_RF_QUEUE_TIMEOUT						0
#define Z_RF_MY_ADDRESS								{0xE7, 0xE7, 0xE7, 0xE7, 0xE7}
#define Z_RF_CAR_ADDRESS							{0x7E, 0x7E, 0x7E, 0x7E, 0x7E}
#define Z_RF_HS_MY_CODE								{0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}
#define Z_RF_HS_CAR_CODE							{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}
	
	RTOS_TIMESTAMP(tsRFMan);
	
	const uint8_t my_address[5] = Z_RF_MY_ADDRESS;
	const uint8_t car_address[5] = Z_RF_CAR_ADDRESS;
	
	const uint8_t my_code[Z_RF_PAYLOAD_SIZE] = Z_RF_HS_MY_CODE;
	const uint8_t car_code[Z_RF_PAYLOAD_SIZE] = Z_RF_HS_CAR_CODE;

	uint8_t data_in[Z_RF_PAYLOAD_SIZE];
	uint8_t data_out[Z_RF_PAYLOAD_SIZE];

	RF_IRQ_t nrf_irq;
	RF_ConnectionState_t conn_state = NOT_CONNECTED;
	
	float meas_angle[2];

	/*!< Wait for all configurations to be complete */
	RTOS_AWAIT(nConfig);
	
	for (int i = 0; i < Z_RF_PAYLOAD_SIZE ; i++)
		data_in[i] = 0xFF;		
	
	/*!< Initialize NRF24L01+ on channel 15 and with a fixed payload size*/
	RF_Init(15, Z_RF_PAYLOAD_SIZE);
	
	/*!<  Set 2MBps data rate and 0Bm output power */
	RF_SetRF(RF_DataRate_2M, RF_OutputPower_0dBm);

	/*!<  Set both addresses */
	RF_SetMyAddress((uint8_t *) my_address);
	RF_SetTxAddress((uint8_t *) car_address);
	
	/*!< Turn on LED_CONN_PROB */
	SET_BIT(LED_CONN_PROB_GPIO_Port->BSRR, LED_CONN_PROB_Pin);
	
	RTOS_KEEP_TIMESTAMP(tsRFMan);
	
	
	////////////////////////////// HANDSHAKE /////////////////////////////////////
		
	/*!< Send request while not connected */
	while(conn_state == NOT_CONNECTED) {
		
		/*!< Send continuous requests until one message is acknowledged */
		do {
			
			/*!< Transmit data, goes automatically to TX mode */
			RF_Transmit((uint8_t*)my_code);
			
			/*!< Await acknowledgement or timeout */
			RTOS_AWAIT_TIMEOUT(nRF, 5);
			
			/*!< Read interrupt flags */
			RF_Read_Interrupts(&nrf_irq);
			
			/*!< If an acknowledgement was received, stop sending */
			if (nrf_irq.F.DataSent)
				break;
			
			/*!< Send RF module to sleep mode */
			RF_PowerDown();
			
			/*!< Wait for the next broadcasting point */
			RTOS_DELAY_UNTIL(tsRFMan, Z_RF_BROADCAST_PERIOD);
			
		} while (!nrf_irq.F.DataSent);
		
		/*!< Enter RX mode */
		RF_PowerUpRx();
		
		/*!< Await reception of the response*/
		RTOS_AWAIT_TIMEOUT(nRF, Z_RF_HS_RESPONSE_TIMEOUT);
						
		/*!< Read interrupt flags */
		RF_Read_Interrupts(&nrf_irq);
		
		/*!< If an acknowledgement was received, stop sending */
		if (nrf_irq.F.DataReady) {
		
			int32_t it;
			
			RF_GetData(data_in);
			
			// TODO: Implement strcmp algo function */
			for (it = Z_RF_PAYLOAD_SIZE; it >= 0; it--) {
			
				if (data_in[it] != car_code[it])
					break;
			}
			
			it++;
			
			if (it == 0)
				conn_state = CONNECTED;
		}
		
		/*!< Termination response */
		if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == 1)
			goto cleanup;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	
	/*!< Send RF module to sleep mode */
	RF_PowerDown();
	
	/*!< Turn off LED_CONN_PROB */
	SET_BIT(LED_CONN_PROB_GPIO_Port->BSRR, (uint32_t)LED_CONN_PROB_Pin<<16);
		
	/*!< Synchronization time stamp */
	RTOS_KEEP_TIMESTAMP(tsConnect);
	tsRFMan = tsConnect;
	
	/*!< Notify tasks of a successful connection */
	RTOS_NOTIFY(zIMUManager, nConnected);
	
	/*!< Initial timing offset */
	RTOS_DELAY_UNTIL(tsRFMan, MASTER_INITIAL_DELAY_MS + Z_RF_PERIOD_MS);
	
	while(1) {
		
		/*!< Get imu readings */
		RTOS_QUEUE_RECV_TIMEOUT(qIMU, meas_angle, Z_RF_QUEUE_TIMEOUT);
		RTOS_QUEUE_RECV_TIMEOUT(qIMU, meas_angle+1, Z_RF_QUEUE_TIMEOUT);
		
		RF_Transmit((uint8_t*)meas_angle);
		
		RTOS_AWAIT_TIMEOUT(nRF, 10);
		
		RF_Read_Interrupts(&nrf_irq);
		
		if (nrf_irq.F.DataSent)
			SET_BIT(LED_CONN_PROB_GPIO_Port->BSRR, (uint32_t)LED_CONN_PROB_Pin<<16);
		else 
			SET_BIT(LED_CONN_PROB_GPIO_Port->BSRR, LED_CONN_PROB_Pin);
		
		/*!< Termination response */
		if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == 1)
			goto cleanup;
		
		RF_PowerDown();
		
		/*!< Sleep */
		RTOS_DELAY_UNTIL(tsRFMan, MASTER_CYCLE_PERIOD_MS);
	}
	
	cleanup:
	RTOS_SEMAPHORE_INC(semTerminate);
	RTOS_TASK_DELETE();
}


void THREADS_create(void) {

	if (F_READ(F_TASKS_CREATED))
		return;

	RTOS_TASK_CREATE_STATIC(zMain);
	RTOS_TASK_CREATE_STATIC(zIMUManager);
	RTOS_TASK_CREATE_STATIC(zRFManager);
	
	RTOS_QUEUE_CREATE_STATIC(qIMU);
	
	RTOS_SEMAPHORE_CREATE_STATIC(semTerminate, 0);
	
	F_SET(F_TASKS_CREATED);
}

void THREADS_startScheduler(void) {

	if (!F_READ(F_TASKS_CREATED) || F_READ(F_SCHEDULER_STARTED))
		return;

	RTOS_SCHEDULER_START();

	F_SET(F_SCHEDULER_STARTED);
}

RTOS_IDLE_CALLBACK() {
	
	/*!< When all tasks have terminated, shut down */
	if (RTOS_SEMAPHORE_GET_COUNT(semTerminate) == RTOS_SEMAPHORE_MAX_COUNT(semTerminate))
		PWR_CLR_PULL_LOW();

	THREADS_sleep();
}

void THREADS_sleep(void){
	
	static uint32_t sleeps = 0;
	
	sleeps++;
	
	/*!< Clear SLEEPDEEP bit of Cortex System Control Register */
	CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

	/*!< Request Wait For Interrupt */
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
