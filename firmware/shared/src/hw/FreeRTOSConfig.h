#pragma once

#if defined(STM32H733xx)
#include "hw_free_rtos_config_h7.h"

#elif defined(STM32H562xx)
#define configENABLE_TRUSTZONE 0
#include "hw_free_rtos_config_h5.h"
#else
#error "Unsupported STM32 target: add the correct FreeRTOS config include for this MCU."

#endif