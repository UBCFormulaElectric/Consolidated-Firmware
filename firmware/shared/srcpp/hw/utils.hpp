#pragma once

#include "hw/hal.hpp"
#include "util/errorCodes.hpp"

#define TICK_TO_MS(tick) ((tick) * portTICK_PERIOD_MS)

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
