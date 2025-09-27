#pragma once

#include "app_utils.h"
#include <stdint.h>

typedef enum
{
    RR_PUMP = 0,
    F_PUMP  = 1
} PUMPS;

ExitCode io_pumpControl_setPercentage(uint8_t percentage, PUMPS pump);

ExitCode io_pumpControl_readPercentage(uint8_t *percentage, PUMPS pump);