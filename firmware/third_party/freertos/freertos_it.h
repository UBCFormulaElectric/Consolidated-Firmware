#pragma once
#include "FreeRTOS.h"
#include "task.h"

#define SVC_Handler ignorethisfunctionplease1
#define PendSV_Handler ignorethisfunctionplease2

static void handle_freertos_it()
{
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif /* INCLUDE_xTaskGetSchedulerState */
        xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    }
#endif /* INCLUDE_xTaskGetSchedulerState */
    /* USER CODE BEGIN SysTick_IRQn 1 */
}