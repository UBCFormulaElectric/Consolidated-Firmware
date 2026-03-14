#pragma once
#include "util_utils.hpp"

CFUNC void adbms_init();
CFUNC void adbms_tick();
void onDmaCfgComplete(); // Called from hw_spi HAL callback when a DMA transaction completes
void onDmaCfgError();    // Called from hw_spi HAL callback when a DMA transaction errors