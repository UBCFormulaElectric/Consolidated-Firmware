#pragma once

#include <stdint.h>

void hw_canConfig_txOverflowCallback(uint32_t overflow_count);
void hw_canConfig_rxOverflowCallback(uint32_t overflow_count);
