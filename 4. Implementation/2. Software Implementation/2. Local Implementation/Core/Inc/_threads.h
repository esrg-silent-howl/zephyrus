#include "_rtos_wrapper.h"

/* Connection State Enum */
typedef enum {
	INITIALIZING=0,
	CONNECTED,
	NOT_CONNECTED
} RF_ConnectionState_t;


/* System Threads Methods Declaration */
RTOS_TASK_FUN(zMain);
RTOS_TASK_FUN(zGyroAccelerometerManager);
RTOS_TASK_FUN(zRFManager);
RTOS_TASK_FUN(zInferenceManager);
RTOS_TASK_FUN(zUltrasonicManager);


void THREADS_create(void);
void THREADS_startScheduler(void);