#include "_rtos_wrapper.h"
#include "tim.h"
#define PWR_CLR_PULL_HIGH()		SET_BIT(PWR_CLR_GPIO_Port->BSRR, PWR_CLR_Pin)
#define PWR_CLR_PULL_LOW()		SET_BIT(PWR_CLR_GPIO_Port->BSRR, PWR_CLR_Pin<<16)

void THREADS_create(void);
void THREADS_startScheduler(void);


void THREADS_shutdownIRQHandler (void);
