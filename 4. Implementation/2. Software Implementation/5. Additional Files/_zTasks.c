/*
 * _zTasks.c
 *
 *  Created on: Dec 22, 2020
 *      Author: josep & Hugoc
 */


#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"

#include "_zTasks.h"
#include "_attr.h"
#include "_rtos_wrapper.h"

//#include "dma.h" /* For later (maybe)*/

/* Set __flag__  (1)*/
#define F_SET(__flag__)			flags |= (uint32_t)(1U<<__flag__)
/* Clear __flag__ (0)*/
#define F_CLEAR(__flag__)		flags &= (uint32_t)(~(1U<<__flag__))
/* Read __flag__   (0 or 1)*/
#define F_READ(__flag__)		((flags & (1<<__flag__)) >> __flag__)

/* No error */
#define E_OK                (uint32_t)0
#define E_STACK_OV          (uint32_t)-1

/* Connection State Enum */
typedef enum {
	INITIALIZING=0,
	CONNECTED,
	NOT_CONNECTED
} CONNECTION_STATE_T;


/* System Threads Initial Delay */
#define INIT_DELAY_MAIN_THREAD	0
#define INIT_DELAY_GYRO_ACC_THREAD		INIT_DELAY_MAIN_THREAD      + 3
#define INIT_DELAY_RF_THREAD	        INIT_DELAY_GYRO_ACC_THREAD  + 3
#define INIT_DELAY_INFERENCE_THREAD    	INIT_DELAY_RF_THREAD        + 3
#define INIT_DELAY_ULTRASONIC_THREAD	INIT_DELAY_INFERENCE_THREAD + 3

/* System Threads Period */
#define PERIOD_MAIN_THREAd          111
#define PERIOD_GYRO_ACC_THREAD      111
#define PERIOD_RF_THREAD            111
#define PERIOD_INFERENCE_THREAD	    111
#define PERIOD_ULTRASONIC_THREAD	111

/*SYSTEM DEFINITION*/
#define STM32F0xx "STM32F0"
#define STM32F7xx "STM32F7"

#if defined(STM32F7xx)
#define ID_FLASH_ADDRESS         0x1FF0F442 /* STM32F7xx address */
#endif
#if defined(STM32F0xx)
#define ID_FLASH_ADDRESS         0x1FFFF7CC /* STM32F0xx address */
#endif

/* Evaluating Flash Memory Size (tested by STMicro)*/
#define GET_FLASH_SIZE()	(*(uint16_t *) (ID_FLASH_ADDRESS)) /* In kilo - read w/ %u*/

/* Verify STM32 - mode 1 */
#if defined STM32F7xx
    #define _LOCAL_ "NUCLEO-S767ZI"
#elif defined STM32F0xx
    #define _REMOTE_ "STM32F051"
#endif

/* Verify STM32 - mode 2 */
#if (GET_FLASH_SIZE() == 512)
    #define _LOCAL_ "NUCLEO-S767ZI"
#elif (GET_FLASH_SIZE() == 32)
    #define _REMOTE_ "STM32F051"
#endif

/*  */

/*!< System Threads Creation */
#if defined (_REMOTE_)
    RTOS_TASK_STATIC(main_thread, 256, RP_IDLE, "zMain");
    RTOS_TASK_STATIC(gyro_acc_thread, 256, RP_NORMAL, "zGyroAccelerometerManager");
    RTOS_TASK_STATIC(rf_thread, 512, RP_ABOVE_NORMAL, "zRFManager");
#endif

#if defined (_LOCAL_)
    RTOS_TASK_STATIC(main_thread, 256, RP_IDLE, "zMain");
    RTOS_TASK_STATIC(gyro_acc_thread, 256, RP_NORMAL, "zGyroAccelerometerManager");
    RTOS_TASK_STATIC(rf_thread, 512, RP_ABOVE_NORMAL, "zRFManager");
    RTOS_TASK_STATIC(inference_thread, 256, RP_HIGH, "zInferenceManager");
    RTOS_TASK_STATIC(ultrasonic_thread, 256, RP_REALTIME, "zUltrasonicManager");
#endif

/* System Threads Methods Declaration */
RTOS_TASK_FUN(main_thread);
RTOS_TASK_FUN(gyro_acc_thread);
RTOS_TASK_FUN(rf_thread);
RTOS_TASK_FUN(inference_thread);
RTOS_TASK_FUN(ultrasonic_thread);

/* Mutexes */
#if defined (_LOCAL_)
    RTOS_MUTEX_STATIC(mState);
    RTOS_MUTEX_STATIC(mReference);
    RTOS_MUTEX_STATIC(mUSS);
#endif
#if defined (_REMOTE_)
//RTOS_MUTEX_STATIC(m...);
#endif

/* Semaphores */
/* (Assumed as a counting semaphores) */
#if defined (_LOCAL_)
    RTOS_SEM_STATIC(semConfig); 
#endif
#if defined (_REMOTE_)
    RTOS_SEM_STATIC(semJoin);
#endif

/* Flags */
#define F_INITIALIZED		0  /*EXAMPLE*/

/* Current connection state */
CONNECTION_STATE_T connection_state;

/* Flags Container*/
uint32_t flags;

/* @brief	Extrapolates the current state */
static void tasks_assess_connection_state();

RTOS_TASK_FUN(main_thread) {
    
    /* Initial Delay */
	vTaskDelay(INIT_DELAY_MAIN_THREAD);

    TickType_t rtos_last_wake_time;

    while(1) {

		/*!< Keep current time stamp */
		rtos_last_wake_time = xTaskGetTickCount();

        /* Local Case */
        #ifndef (_REMOTE_)
        #if defined (_LOCAL_)
    
        #endif
        #endif

        /* Remote Case */
        #ifndef (_LOCAL_)
        #if defined (_REMOTE_)

        #endif
        #endif

        /*!< Sleep */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_MAIN_THREAD);
	}

	vTaskDelete(NULL);

}

RTOS_TASK_FUN(gyro_acc_thread) {

    /* Initial Delay */
	vTaskDelay(INIT_DELAY_GYRO_ACC_THREAD);

    TickType_t rtos_last_wake_time;

    while(1) {

		/*!< Keep current time stamp */
		rtos_last_wake_time = xTaskGetTickCount();

        /* Local Case */
        #ifndef (_REMOTE_)
        #if defined (_LOCAL_)
    
        #endif
        #endif

        /* Remote Case */
        #ifndef (_LOCAL_)
        #if defined (_REMOTE_)

        #endif
        #endif

        /*!< Sleep */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_GYRO_ACC_THREAD);
	}

	vTaskDelete(NULL);


}

RTOS_TASK_FUN(rf_thread) {

    /* Initial Delay */
	vTaskDelay(INIT_DELAY_RF_THREAD);

    TickType_t rtos_last_wake_time;

    while(1) {

		/*!< Keep current time stamp */
		rtos_last_wake_time = xTaskGetTickCount();

        /* Local Case */
        #ifndef (_REMOTE_)
        #if defined (_LOCAL_)
    
        #endif
        #endif

        /* Remote Case */
        #ifndef (_LOCAL_)
        #if defined (_REMOTE_)

        #endif
        #endif

        /*!< Sleep */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_RF_THREAD);
	}

	vTaskDelete(NULL);

}

RTOS_TASK_FUN(inference_thread) {

    /* Initial Delay */
	vTaskDelay(INIT_DELAY_INFERENCE_THREAD);

    TickType_t rtos_last_wake_time;

    while(1) {

		/*!< Keep current time stamp */
		rtos_last_wake_time = xTaskGetTickCount();
    
        /* Local Case */
        #ifndef (_REMOTE_)
        #if defined (_LOCAL_)


        #endif
        #endif

        /*!< Sleep */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_INFERENCE_THREAD);
	}

	vTaskDelete(NULL);

}

RTOS_TASK_FUN(ultrasonic_thread) {

    /* Initial Delay */
	vTaskDelay(INIT_DELAY_ULTRASONIC_THREAD);

    TickType_t rtos_last_wake_time;

    while(1) {

		/*!< Keep current time stamp */
		rtos_last_wake_time = xTaskGetTickCount();
    
        /* Local Case */
        #ifndef (_REMOTE_)
        #if defined (_LOCAL_)


        #endif
        #endif

        /*!< Sleep */
		vTaskDelayUntil(&rtos_last_wake_time, PERIOD_ULTRASONIC_THREAD);
	}

	vTaskDelete(NULL);
}

void tasks_create(void) {

#if defined (_REMOTE_)
	RTOS_TASK_CREATE_STATIC(main_thread);
    RTOS_TASK_CREATE_STATIC(gyro_acc_thread);
    RTOS_TASK_CREATE_STATIC(rf_thread);
#endif

#if defined (_LOCAL_)
	RTOS_TASK_CREATE_STATIC(main_thread);
	RTOS_TASK_CREATE_STATIC(gyro_acc_thread);
	RTOS_TASK_CREATE_STATIC(rf_thread);
	RTOS_TASK_CREATE_STATIC(inference_thread);
    RTOS_TASK_CREATE_STATIC(ultrasonic_thread);
#endif


#if defined (_LOCAL_)
    RTOS_MUTEX_CREATE_STATIC(mState);
    RTOS_MUTEX_CREATE_STATIC(mReference);
    RTOS_MUTEX_CREATE_STATIC(mUSS);
#endif
#if defined (_REMOTE_)

#endif


#if defined (_LOCAL_)
    RTOS_SEM_CREATE_STATIC(semConfig);
#endif
#if defined (_REMOTE_)
    RTOS_SEM_CREATE_STATIC(semJoin);
#endif
}

void tasks_start_scheduler(void) {

	vTaskStartScheduler();
}




