#include "_rtos_wrapper.h"

/*!< System Threads Priority */
#define PRIO_MAIN	RP_IDLE
#define PRIO_GYRO_ACCELEROMETER_MANAGER	RP_LOW
#define PRIO_RF_MANAGER	RP_HIGH
#define PRIO_INFERENCE_MANAGER	RP_BELOW_NORMAL
/*falta o resto das tasks*/

/*!< System Threads Methods Declaration */
RTOS_TASK_FUN(ZMain);
RTOS_TASK_FUN(zGyroAccelerometerManager);
RTOS_TASK_FUN(zRFManager);
RTOS_TASK_FUN(zUltrasonicManager);
RTOS_TASK_FUN(zInferenceManager);
RTOS_TASK_FUN(zPowerManager);