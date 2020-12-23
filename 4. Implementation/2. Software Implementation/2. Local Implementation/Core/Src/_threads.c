#include "_threads.h"
#include "main.h"

/*!< Flag definitions */
#define F_TASKS_CREATED		0
#define F_SCHEDULER_STARTED	(F_TASKS_CREATED+1)

/*!< No error */
#define E_OK				(uint32_t)0
/*!< Error in message size */
#define E_MSG_SIZE	(uint32_t)(-1)
/*!< Error in number of sets */
#define E_SET_COUNT	(uint32_t)(-2)

/*!< System Threads Period */
#define PERIOD_MAIN_THREAD          xxxx
#define PERIOD_GYRO_ACC_THREAD      xxxx
#define PERIOD_RF_THREAD            xxxx
#define PERIOD_INFERENCE_THREAD	    xxxx
#define PERIOD_ULTRASONIC_THREAD	xxxx

/*!< System Threads Creation */
RTOS_TASK_STATIC(zMain, 256, RP_IDLE, "zMain");
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
	
static void THREADS_checkForPower(void){


}
	

RTOS_TASK_FUN(zMain) {
    
	RTOS_TIMESTAMP(timestamp);

	while(1) {

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Read Power Pin */
		THREADS_checkForPower();
		
		/*!< Check for working threads */
		while(RTOS_SEMAPHORE_GET_COUNT(semWorking) != 0){};
			
		THREADS_sleep(); /*!< TODO: Sleep */
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

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_DEC(semWorking);
		
		/*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, 5);
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

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    /*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, 5);
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

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    /*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, 5);
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

		/*!< Keep current time stamp */
		RTOS_KEEP_TIMESTAMP(timestamp);
		
		/*!< Declare the sleep mode entering */
		RTOS_SEMAPHORE_DEC(semWorking);
		
    /*!< Sleep */
		RTOS_DELAY_UNTIL(timestamp, 5);
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