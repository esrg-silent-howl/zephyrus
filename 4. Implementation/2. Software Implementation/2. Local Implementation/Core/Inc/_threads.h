#include "_rtos_wrapper.h"
#include "tim.h"


void THREADS_create(void);
void THREADS_startScheduler(void);

void THREADS_tim2IRQHandler(void);
void THREADS_rfIRQHandler (void);
