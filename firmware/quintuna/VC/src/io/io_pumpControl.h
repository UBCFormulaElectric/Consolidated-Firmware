#pragma once

#include "app_utils.h"
#include "io_pump.h"

#include <stdint.h>

void io_pumpControl_init(void);

ExitCode io_pumpControl_setPercentage(uint8_t percentage, PumpID pump);
ExitCode io_pumpControl_readPercentage(uint8_t *percentage, PumpID pump);

