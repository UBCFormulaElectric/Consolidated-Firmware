#pragma once

#if defined(STM32H733xx)
#include "FreeRTOSConfig_H7.h"
#elif defined(STM32H562xx)
#define configENABLE_TRUSTZONE 0
#include "FreeRTOSConfig_H5.h"
#else
#error "Unsupported STM32 target: add the correct FreeRTOS config include for this MCU."
#endif

#include "SEGGER_SYSVIEW_FreeRTOS.h"

#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_pxTaskGetStackStart 1
#define INCLUDE_vTaskDelayUntil 1
#define configCHECK_FOR_STACK_OVERFLOW 2

#define configRECORD_STACK_HIGH_ADDRESS 1

#ifdef PROFILING_ENABLED
// tracing shit
#if __cplusplus
extern "C"
{
#endif
    void          configureTimerForRunTimeStats();
    unsigned long getRunTimeCounterValue();
#if __cplusplus
}
#endif
#define configUSE_TRACE_FACILITY 1
#define configGENERATE_RUN_TIME_STATS 1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS configureTimerForRunTimeStats
#define portGET_RUN_TIME_COUNTER_VALUE getRunTimeCounterValue
#endif