#pragma once

#include "SEGGER_SYSVIEW_FreeRTOS.h"

#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_pxTaskGetStackStart 1
#define INCLUDE_vTaskDelayUntil 1
#define configCHECK_FOR_STACK_OVERFLOW 2

#define configUSE_IDLE_HOOK 1
#define configUSE_TICK_HOOK 1
#define configRECORD_STACK_HIGH_ADDRESS 1
