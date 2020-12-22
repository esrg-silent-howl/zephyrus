/*!
 *******************************************************************************
 * @file		_rtos_wrapper.h
 * @brief		Set of wrappers for functions and definitions related to the RTOS
 * in use
 */

/** @addtogroup RTOS_WRAPPER
 *  @{
 */

#ifndef INC__RTOS_WRAPPER_H_
#define INC__RTOS_WRAPPER_H_

#include "cmsis_os.h"


/*!
 *	@brief	Concatenates x and y. It is meant to support other macro definitions.
 *
 *	<b>Example: </b>\n
 *	<b>CAT(x, Task)</b> yields the result: \n
 *	<b>xTask</b>
 *
 *	@param	x
 *	@param	y
 */
#define CAT(x, y) x ## y

/*! Returned by some methods as a way to convey success in their operation */
#define RTOS_RETURN_SUCCESS pdTRUE

/*! Returned by some methods as a way to convey failure in their operation */
#define RTOS_RETURN_FAILURE pdFALSE

/*! Converts a value from milliseconds to its equivalent in system ticks */
#define RTOS_TOOLS_MS_TO_TICKS(__ms__)	pdMS_TO_TICKS(__ms__)

/*!< Notifies a task */
#define RTOS_NOTIFICATION_GIVE(__task__) \
xTaskNotifyGive(RTOS_TASK_HANDLE(__task__))

/*!< Awaits a notification */
#define RTOS_NOTIFICATION_WAIT()		ulTaskNotifyTake(pdTRUE, portMAX_DELAY)

/*!
 *	@brief	Get the representation of the mutex buffer associated with __mutex__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_MUTEX_BUFFER(xTask)</b> yields the result: \n
 *	<b>xTask_MutexBuffer</b>
 *
 *	@param	__mutex__				mutex name
 */
#define RTOS_MUTEX_BUFFER(__mutex__)	CAT(__mutex__, _MutexBuffer)

/*!
 *	@brief	Get the representation of the handle associated with __mutex__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_MUTEX_HANDLE(xMutex)</b> yields the result: \n
 *	<b>xMutex</b>
 *
 *	@param	__mutex__	mutex name
 */
#define RTOS_MUTEX_HANDLE(__mutex__)	__mutex__

/*!
 *	@brief	Declare the necessary objects for creating and manipulating a mutex.
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_MUTEX_CREATE_STATIC(xMutex)</b> yields the result: \n
 *	<b>SemaphoreHandle_t xMutex;\n
 *	StaticSemaphore_t xMutex_MutexBuffer;</b>
 *
 *	@param	__mutex__				mutex name
 */
#define RTOS_MUTEX_STATIC(__mutex__) \
SemaphoreHandle_t RTOS_MUTEX_HANDLE(__mutex__); \
StaticSemaphore_t RTOS_MUTEX_BUFFER(__mutex__);

/*!
 *	@brief	Wrapper macro for 'xSemaphoreCreateMutexStatic'
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_CREATE_STATIC(xMutex)</b> yields the result: \n
 *	<b>xSemaphoreCreateMutexStatic(&xMutex_MutexBuffer)
 *
 *	@param	__mutex__				mutex name
 */
#define RTOS_MUTEX_CREATE_STATIC(__mutex__)	\
	RTOS_MUTEX_HANDLE(__mutex__) = xSemaphoreCreateMutexStatic( \
			&RTOS_MUTEX_BUFFER(__mutex__))

/*!
 *	@brief	Wrapper macro for 'xSemaphoreTake' for specific use for mutexes
 *	created through this API
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_MUTEX_LOCK(xMutex, 20)</b> yields the result: \n
 *	<b>xSemaphoreTake(xMutex, 20)
 *
 *	@param	__task__				task name
 *	@return
 *	 - RTOS_RETURN_SUCCESS in case the mutex is or becomes available
 *	 - RTOS_RETURN_FAILURE in case the mutex is not available before timeout
 */
#define RTOS_MUTEX_LOCK(__mutex__, __ticks_to_wait__) \
	xSemaphoreTake(__mutex__, __ticks_to_wait__)

/*!
 *	@brief	Wrapper macro for 'xSemaphoreTake' for specific use for mutexes
 *	created through this API
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_MUTEX_LOCK(xMutex, 20)</b> yields the result: \n
 *	<b>xSemaphoreTake(xMutex, 20)
 *
 *	@param	__task__				task name
 *	@return
 *	 - RTOS_RETURN_SUCCESS if the mutex was released successfully
 *	 - RTOS_RETURN_FAILURE if an error occured
 */
#define RTOS_MUTEX_UNLOCK(__mutex__) \
	xSemaphoreGive(__mutex__)



/*!
 *	@brief	Get the representation of the stack array associated with __task__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_STACK(xTask)</b> yields the result: \n
 *	<b>xTask_Stack</b>
 *
 *	@param	__task__	task name
 */
#define RTOS_TASK_STACK(__task__)		CAT(__task__, _Stack)

/*!
 *	@brief	Get the representation of the handle associated with __task__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_HANDLE(xTask)</b> yields the result: \n
 *	<b>xTask</b>
 *
 *	@param	__task__	task name
 */
#define RTOS_TASK_HANDLE(__task__)	__task__

/*!
 *	@brief	Get the representation of the function associated with __task__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_FUNCTION(xTask)</b> yields the result: \n
 *	<b>xTask_Fun</b>
 *
 *	@param	__task__	task name
 */
#define RTOS_TASK_FUNCTION(__task__)	CAT(__task__,_Fun)


/*!
 *	@brief	Get the representation of the stack buffer associated with __task__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_BUFFER(xTask)</b> yields the result: \n
 *	<b>xTask_TaskBuffer</b>
 *
 *	@param	__task__				task name
 */
#define RTOS_TASK_BUFFER(__task__)	CAT(__task__, _TaskBuffer)

/*!
 *	@brief	Get the representation of the stack size associated with __task__
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_STACK_SIZE(xTask)</b> yields the result: \n
 *	<b>xTask_StackSize</b>
 *
 *	@param	__task__				task name
 */
#define RTOS_TASK_STACK_SIZE(__task__)	CAT(__task__, _StackSize)

/*!
 *	@brief	Get the representation of __task__'s priority
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_PRIORITY(xTask)</b> yields the result: \n
 *	<b>xTask_Priority</b>
 *
 *	@param	__task__				task name
 */
#define RTOS_TASK_PRIORITY(__task__)	CAT(__task__, _Priority)

/*!
 *	@brief	Get the representation of __task__'s informal name
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_NAME(xTask)</b> yields the result: \n
 *	<b>xTask_Name</b>
 *
 *	@param	__task__				task name
 */
#define RTOS_TASK_NAME(__task__)	CAT(__task__, _Name)

/*!
 *	@brief	Declare the necessary objects for creating and manipulating a task.
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_CREATE_STATIC(xTask, 256, RP_NORMAL, "x_task")</b>
 *	yields the result: \n
 *	<b>const uint32_t xTask_StackSize = 256;\n
 *	const uint32_t xTask_Priority = RP_NORMAL;\n
 *	const char xTask_Name[] = "x_task";\n
 *	StaticTask_t xTask_TaskBuffer;\n
 *	StackType_t xTask_Stack[256];\n
 *	TaskHandle_t xTask;</b>
 *
 *	@param	__task__				task name
 *	@param	__STACK_SIZE__	stack size
 *	@param __PRIORITY__			task priority (of enum type RTOSPriorities_t)
 *	@param __NAME__					task name (informal name in const char* format)
 *
 */
#define RTOS_TASK_STATIC(__task__, __STACK_SIZE__, __PRIORITY__, __NAME__) \
const uint32_t RTOS_TASK_STACK_SIZE(__task__) = __STACK_SIZE__/4;\
const uint32_t RTOS_TASK_PRIORITY(__task__) = __PRIORITY__;\
const char RTOS_TASK_NAME(__task__)[] = __NAME__;\
StaticTask_t RTOS_TASK_BUFFER(__task__);\
StackType_t RTOS_TASK_STACK(__task__)[__STACK_SIZE__/4];\
TaskHandle_t RTOS_TASK_HANDLE(__task__);

/*!
 *	@brief	Wrapper macro for 'xTaskCreateStatic'
 *
 *	<b>Example: </b>\n
 *	<b>RTOS_TASK_CREATE_STATIC(xTask, 256, RP_NORMAL, "x_task")\n
 *	RTOS_TASK_CREATE_STATIC(xTask)</b>\n
 *	yields the result: \n
 *	<b>RTOS_TASK_HANDLE(xTask) =  xTaskCreateStatic(xTask, "x_task", 256, NULL,
 *	RP_NORMAL, xTask_Stack, xTask_TaskBuffer)</b>
 *
 *	@param	__task__				task name
 */
#define RTOS_TASK_CREATE_STATIC(__task__)	\
	RTOS_TASK_HANDLE(__task__) = xTaskCreateStatic(\
			RTOS_TASK_FUNCTION(__task__), \
			RTOS_TASK_NAME(__task__), \
			RTOS_TASK_STACK_SIZE(__task__),\
			NULL, RTOS_TASK_PRIORITY(__task__), \
			RTOS_TASK_STACK(__task__), \
			&RTOS_TASK_BUFFER(__task__))

/*!
 *	@brief	Default task parameter
 */
#define RTOS_TASK_STATIC_DEFAULT_PARAM	void* empty_param

/*!
 *	@brief	Declare task function
 *	@param	__task__	name of the task
 */
#define RTOS_TASK_FUN(__task__)	\
	void CAT(__task__,_Fun)(RTOS_TASK_STATIC_DEFAULT_PARAM)

/*!
 *	@brief	RTOS priorities enumeration
 */
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


/** @}*/

