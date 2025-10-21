#pragma once

#include "hw/hal.hpp"
#include "util/errorCodes.hpp"

#define TICK_TO_MS(tick) ((tick) * portTICK_PERIOD_MS)

/* @brief Set a software breakpoint if debugger is connected */
#define BREAK_IF_DEBUGGER_CONNECTED()                     \
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) \
    {                                                     \
        __asm__ __volatile__("bkpt #0");                  \
    }

inline ExitCode hw_utils_convertHalStatus(const HAL_StatusTypeDef status)
{
    switch (status)
    {
        case HAL_OK:
            return ExitCode::EXIT_CODE_OK;
        case HAL_ERROR:
            return ExitCode::EXIT_CODE_ERROR;
        case HAL_BUSY:
            return ExitCode::EXIT_CODE_BUSY;
        case HAL_TIMEOUT:
            return ExitCode::EXIT_CODE_TIMEOUT;
        default:
            return ExitCode::EXIT_CODE_OK;
    }
}
