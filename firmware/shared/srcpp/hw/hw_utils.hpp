#pragma once

#include "hw_hal.hpp"
#include "util_errorCodes.hpp"

#define TICK_TO_MS(tick) ((tick) * portTICK_PERIOD_MS)

/* @brief Set a software breakpoint if debugger is connected */
#define BREAK_IF_DEBUGGER_CONNECTED()                     \
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) \
    {                                                     \
        __asm__ __volatile__("bkpt #0");                  \
    }

<<<<<<< HEAD static ExitCode hw_utils_convertHalStatus(const HAL_StatusTypeDef status)
=======
    inline std::expected<void, ErrorCode> hw_utils_convertHalStatus(const HAL_StatusTypeDef status)
>>>>>>> master
{
    switch (status)
    {
        case HAL_ERROR:
            return std::unexpected(ErrorCode::ERROR);
        case HAL_BUSY:
            return std::unexpected(ErrorCode::BUSY);
        case HAL_TIMEOUT:
            return std::unexpected(ErrorCode::TIMEOUT);
        case HAL_OK:
        default:
            return {};
    }
}
