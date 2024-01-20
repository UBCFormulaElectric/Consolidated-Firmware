#pragma once

#include <stdint.h>

void io_canConfig_txOverflowCallback(uint32_t overflow_count);
void io_canConfig_rxOverflowCallback(uint32_t overflow_count);
