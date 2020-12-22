#include "_scheduler.h"
#include "_threads.h"

/*!< System Threads Creation */
RTOS_TASK_STATIC(ZMain, 0, PRIO_MAIN, "ZMain");
RTOS_TASK_STATIC(zGyroAccelerometerManager, 0, PRIO_GYRO_ACCELEROMETER_MANAGER, "zGyroAccelerometerManager");
RTOS_TASK_STATIC(zRFManager, 0, PRIO_RF_MANAGER, "zRFManager");
RTOS_TASK_STATIC(zInferenceManager, 0, RP_BELOW_NORMAL, "zInferenceManager");

/*!< Mutex for shared access to the variable 'sets: set_t[]'  */
RTOS_MUTEX_STATIC(/*name*/);


/* falta fazer o wrapper e declaracao dos semaphores e das notifications */

void scheduler_create_tasks(void) {
	
	RTOS_TASK_CREATE_STATIC(ZMain);
	RTOS_TASK_CREATE_STATIC(zGyroAccelerometerManager);
	RTOS_TASK_CREATE_STATIC(zRFManager);
	RTOS_TASK_CREATE_STATIC(zInferenceManager);
	RTOS_TASK_CREATE_STATIC(zUltrasonicManager);
	RTOS_TASK_CREATE_STATIC(zPowerManager);
	
	RTOS_MUTEX_CREATE_STATIC(/*name*/);
}

void scheduler_start(void) {
	
	vTaskStartScheduler();
}

