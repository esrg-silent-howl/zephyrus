#include "_rtos_wrapper.h"
#include "tim.h"

/* Connection State Enum */
typedef enum {
	INITIALIZING=0,
	CONNECTED,
	NOT_CONNECTED
} RF_ConnectionState_t;


void THREADS_create(void);
void THREADS_startScheduler(void);

void THREADS_tim2IRQHandler();