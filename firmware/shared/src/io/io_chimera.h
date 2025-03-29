#pragma once

#include <stdint.h>

void io_chimera_init(uint32_t name_gpio, uint32_t name_adc);
void io_chimera_msgRxCallback(void);
void io_chimera_sleepTaskIfEnabled(void);
