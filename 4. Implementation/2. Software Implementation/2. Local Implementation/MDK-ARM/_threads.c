#include "_threads.h"

RTOS_TASK_FUN(flash_thread) {

	TickType_t rtos_last_wake_time;
	
	/*!< Initial delay */
	vTaskDelay(INIT_DELAY_FLASH_THREAD);

	while(1) {
		/*!< Keep current timestamp */
		rtos_last_wake_time = xTaskGetTickCount();
		
		/*!< Zzzzz */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_SERIAL_THREAD);
	}

	vTaskDelete(NULL);
}
