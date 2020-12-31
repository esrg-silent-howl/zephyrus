#ifndef INC__RTOS_WRAPPER_H_
#define INC__RTOS_WRAPPER_H_

#include "cmsis_os.h"

/*!< --------------------------------- TOOLS -------------------------------- */

/*! Concatenates x and y. It is meant to support other macro definitions */
#define CAT(x, y) x ## y

/*! Returned by some methods as a way to convey success in their operation */
#define RTOS_TRUE			pdTRUE

/*! Returned by some methods as a way to convey failure in their operation */
#define RTOS_FALSE		pdFALSE

/*! Returned by some methods as a way to convey a timeout in their operation */
#define RTOS_TIMEOUT	pdFALSE

/*! Converts a value from milliseconds to its equivalent in system ticks */
#define RTOS_TOOLS_MS_TO_TICKS(__ms__)	pdMS_TO_TICKS(__ms__)


/*!< ------------------------------- SCHEDULER ------------------------------ */

#define RTOS_SCHEDULER_START() \
	vTaskStartScheduler();

/*!< -------------------------------- DELAYS -------------------------------- */

#define RTOS_TIMESTAMP(__timestamp__)	\
	TickType_t __timestamp__

#define RTOS_KEEP_TIMESTAMP(__timestamp__) \
	__timestamp__ = xTaskGetTickCount()
	
#define RTOS_GET_TIMESTAMP() \
	xTaskGetTickCount()

/*!< Delay task execution __msdelay__ ms */
#define RTOS_DELAY(__msdelay__)	\
	vTaskDelay(RTOS_TOOLS_MS_TO_TICKS(__msdelay__))

/*!< Delay task execution __msdelay__ ms past __timestamp__*/
#define RTOS_DELAY_UNTIL(__timestamp__, __msdelay__)	\
	vTaskDelayUntil(&__timestamp__, RTOS_TOOLS_MS_TO_TICKS(__msdelay__))


/*!< ----------------------------- NOTIFICATIONS ---------------------------- */

/*!< Create a notification with an __id__ in the range [0, 31] */
#define RTOS_NOTIFICATION(__notification__, __id__) \
uint32_t __notification__ = (1 << __id__);

/*!< Notifies a task */
#define RTOS_NOTIFY(__task__, __notification__) \
xTaskNotify(__task__, __notification__, eSetBits)

/*!< Notifies a task */
#define RTOS_NOTIFY_ISR(__task__, __notification__) \
xTaskNotifyFromISR(__task__, __notification__, eSetBits, NULL)

/*!< Awaits a notification */
#define RTOS_AWAIT(__notification__)		\
xTaskNotifyWait(0, __notification__, NULL, portMAX_DELAY)

/*!< Awaits a notification */
#define RTOS_AWAIT_TIMEOUT(__notification__, __timeout__)		\
xTaskNotifyWait(0, __notification__, NULL, RTOS_TOOLS_MS_TO_TICKS(__timeout__))


/*!< ------------------------------ SEMAPHORES ------------------------------ */

/*!< Get the representation of the mutex buffer associated with __semaphore__ */
#define RTOS_SEMAPHORE_BUFFER(__semaphore__)	\
	CAT(__semaphore__, _SemaphoreBuffer)

/*!< Get the representation of the handle associated with __semaphore__ */
#define RTOS_SEMAPHORE_HANDLE(__semaphore__)	\
	__semaphore__

/*!< Get __semaphore__'s maximum count */
#define RTOS_SEMAPHORE_MAX_COUNT(__semaphore__) \
	CAT(__semaphore__, _MaxCount)

/*!< Declare the necessary objects for creating and manipulating a mutex */
#define RTOS_SEMAPHORE_STATIC(__semaphore__, __MAX_COUNT__) \
const uint32_t RTOS_SEMAPHORE_MAX_COUNT(__semaphore__) = __MAX_COUNT__; \
SemaphoreHandle_t RTOS_SEMAPHORE_HANDLE(__semaphore__); \
StaticSemaphore_t RTOS_SEMAPHORE_BUFFER(__semaphore__);

/*!< Wrapper macro for 'xSemaphoreCreateMutexStatic' */
#define RTOS_SEMAPHORE_CREATE_STATIC(__semaphore__, __INIT_COUNT__)	\
	RTOS_SEMAPHORE_HANDLE(__semaphore__) = xSemaphoreCreateCountingStatic( \
			RTOS_SEMAPHORE_MAX_COUNT(__semaphore__), __INIT_COUNT__, \
			&RTOS_SEMAPHORE_BUFFER(__semaphore__))

/*!< Wrapper macro for 'xSemaphoreTake' for specific use for semaphores
 *	created through this API */
#define RTOS_SEMAPHORE_DEC(__semaphore__) \
	xSemaphoreTake(__semaphore__, portMAX_DELAY)

/*!< Wrapper macro for 'xSemaphoreGive' for specific use for semaphores
 *	created through this API */
#define RTOS_SEMAPHORE_INC(__semaphore__) \
	xSemaphoreGive(__semaphore__)
	
	/*!< Wrapper macro for 'xSemaphoreTakeFromISR' for specific use for semaphores
 *	created through this API */
#define RTOS_SEMAPHORE_INC_ISR(__semaphore__) \
	xSemaphoreGiveFromISR(__semaphore__, NULL)

/*!< Wrapper macro for 'xSemaphoreGiveFromISR' for specific use for semaphores
 *	created through this API */
#define RTOS_SEMAPHORE_DEC_ISR(__semaphore__) \
	xSemaphoreTakeFromISR(__semaphore__, NULL)

/*!< Get count of __semaphore__ */
#define RTOS_SEMAPHORE_GET_COUNT(__semaphore__) \
	uxSemaphoreGetCount(RTOS_SEMAPHORE_HANDLE(__semaphore__))


/*!< -------------------------------- QUEUES -------------------------------- */

#define RTOS_QUEUE_BUFFER(__queue__) \
	CAT(__queue__, _QueueBuffer)

#define RTOS_QUEUE_SIZE(__queue__) \
	CAT(__queue__, _QueueSize)

#define RTOS_QUEUE_HANDLE(__queue__)	\
	__queue__
	
#define RTOS_QUEUE_STORAGE(__queue__) \
	CAT(__queue__, _QueueStorage)
	
#define RTOS_QUEUE_TYPE_SIZE(__queue__) \
	CAT(__queue__, _QueueTypeSize)
	
#define RTOS_QUEUE_STATIC(__queue__, __type__, __size__)	\
	QueueHandle_t RTOS_QUEUE_HANDLE(__queue__); \
	StaticQueue_t RTOS_QUEUE_BUFFER(__queue__); \
	const uint32_t RTOS_QUEUE_SIZE(__queue__) = __size__; \
	const uint32_t RTOS_QUEUE_TYPE_SIZE(__queue__) = sizeof(__type__); \
	__type__ RTOS_QUEUE_STORAGE(__queue__)[__size__];
	
#define RTOS_QUEUE_CREATE_STATIC(__queue__) \
	RTOS_QUEUE_HANDLE(__queue__) = xQueueCreateStatic(RTOS_QUEUE_SIZE(__queue__), \
			RTOS_QUEUE_TYPE_SIZE(__queue__), (uint8_t*)RTOS_QUEUE_STORAGE(__queue__), \
			&RTOS_QUEUE_BUFFER(__queue__));

#define RTOS_QUEUE_RECV(__queue__, __dest_ptr__) \
	xQueueReceive(__queue__, __dest_ptr__, portMAX_DELAY)
	
#define RTOS_QUEUE_RECV_TIMEOUT(__queue__, __dest_ptr__, __timeout__) \
	xQueueReceive(__queue__, __dest_ptr__, RTOS_TOOLS_MS_TO_TICKS(__timeout__))

#define RTOS_QUEUE_SEND_BACK(__queue__, __source__) \
	xQueueSendToBack(__queue__, __source__, portMAX_DELAY)
	
#define RTOS_QUEUE_SEND_BACK_TIMEOUT(__queue__, __source__, __timeout__) \
	xQueueSendToBack(__queue__, __source__, RTOS_TOOLS_MS_TO_TICKS(__timeout__))

#define RTOS_QUEUE_SEND_BACK_ISR(__queue__) \
	xQueueSendToBackFromISR(__queue__, __source__, NULL)

#define RTOS_QUEUE_IS_FULL(__queue__) \
	(uxQueueMessagesWaiting(__queue__) == RTOS_QUEUE_SIZE(__queue__))

#define RTOS_QUEUE_IS_FULL_ISR(__queue__) \
	(uxQueueMessagesWaitingFromISR(__queue__) == RTOS_QUEUE_SIZE(__queue__))

#define RTOS_QUEUE_IS_EMPTY(__queue__) \
	(uxQueueMessagesWaiting(__queue__) == 0)
	
#define RTOS_QUEUE_IS_EMPTY_ISR(__queue__) \
	(uxQueueMessagesWaitingFromISR(__queue__) == 0)
	
/*!< -------------------------------- MUTEXES ------------------------------- */

/*! Get the representation of the mutex buffer associated with __mutex__ */
#define RTOS_MUTEX_BUFFER(__mutex__)	CAT(__mutex__, _MutexBuffer)

/*! Get the representation of the handle associated with __mutex__ */
#define RTOS_MUTEX_HANDLE(__mutex__)	__mutex__

/*! Declare the necessary objects for creating and manipulating a mutex. */
#define RTOS_MUTEX_STATIC(__mutex__) \
SemaphoreHandle_t RTOS_MUTEX_HANDLE(__mutex__); \
StaticSemaphore_t RTOS_MUTEX_BUFFER(__mutex__);

/*! Wrapper macro for 'xSemaphoreCreateMutexStatic' */
#define RTOS_MUTEX_CREATE_STATIC(__mutex__)	\
	RTOS_MUTEX_HANDLE(__mutex__) = xSemaphoreCreateMutexStatic( \
			&RTOS_MUTEX_BUFFER(__mutex__))

/*! Wrapper macro for 'xSemaphoreTake' for specific use for mutexes
 *	created through this API */
#define RTOS_MUTEX_LOCK(__mutex__) \
	xSemaphoreTake(__mutex__, portMAX_DELAY)

/*! Wrapper macro for 'xSemaphoreTake' for specific use for mutexes */
#define RTOS_MUTEX_UNLOCK(__mutex__) \
	xSemaphoreGive(__mutex__)


/*!< --------------------------------- TASKS -------------------------------- */

/*! Get the representation of the stack array associated with __task__ */
#define RTOS_TASK_STACK(__task__)		CAT(__task__, _Stack)

/*! Get the representation of the handle associated with __task__ */
#define RTOS_TASK_HANDLE(__task__)	\
	__task__

/*! Get the representation of the function associated with __task__ */
#define RTOS_TASK_FUNCTION(__task__)	\
	CAT(__task__,_Fun)

/*! Get the representation of the stack buffer associated with __task__ */
#define RTOS_TASK_BUFFER(__task__) \
	CAT(__task__, _TaskBuffer)

/*! Get the representation of the stack size associated with __task__ */
#define RTOS_TASK_STACK_SIZE(__task__) \
	CAT(__task__, _StackSize)

/*! Get the representation of __task__'s priority */
#define RTOS_TASK_PRIORITY(__task__) \
	CAT(__task__, _Priority)

/*! Get the representation of __task__'s informal name */
#define RTOS_TASK_NAME(__task__) \
	CAT(__task__, _Name)

/*! Declare the necessary objects for creating and manipulating a task. */
#define RTOS_TASK_STATIC(__task__, __STACK_SIZE__, __PRIORITY__, __NAME__) \
	const uint32_t RTOS_TASK_STACK_SIZE(__task__) = __STACK_SIZE__/4;\
	const uint32_t RTOS_TASK_PRIORITY(__task__) = __PRIORITY__;\
	const char RTOS_TASK_NAME(__task__)[] = __NAME__;\
	StaticTask_t RTOS_TASK_BUFFER(__task__);\
	StackType_t RTOS_TASK_STACK(__task__)[__STACK_SIZE__/4];\
	TaskHandle_t RTOS_TASK_HANDLE(__task__);

/*! Wrapper macro for 'xTaskCreateStatic' */
#define RTOS_TASK_CREATE_STATIC(__task__)	\
	RTOS_TASK_HANDLE(__task__) = xTaskCreateStatic(\
			RTOS_TASK_FUNCTION(__task__), \
			RTOS_TASK_NAME(__task__), \
			RTOS_TASK_STACK_SIZE(__task__),\
			NULL, RTOS_TASK_PRIORITY(__task__), \
			RTOS_TASK_STACK(__task__), \
			&RTOS_TASK_BUFFER(__task__))

/*! Default task parameter */
#define RTOS_TASK_STATIC_DEFAULT_PARAM	void* empty_param

/*! Declare task function */
#define RTOS_TASK_FUN(__task__)	\
	void CAT(__task__,_Fun)(RTOS_TASK_STATIC_DEFAULT_PARAM)

#define RTOS_TASK_DELETE() \
	vTaskDelete(NULL)
	
#define RTOS_IDLE_CALLBACK()	\
	void vApplicationIdleHook(void)
	
/*! RTOS priorities enumeration */
typedef enum RTOSPriorities {
	RP_IDLE = 0,			/*!< IDLE priority */
	RP_LOW,						/*!< LOW priority */
	RP_BELOW_NORMAL,	/*!< BELOW NORMAL priority */
	RP_NORMAL,				/*!< NORMAL priority */
	RP_ABOVE_NORMAL,	/*!< ABOVE NORMAL priority */
	RP_HIGH,					/*!< HIGH priority */
	RP_REAL_TIME			/*!< REAL TIME priority */
} RTOSPriorities_t;


#endif /* INC__RTOS_WRAPPER_H_ */

