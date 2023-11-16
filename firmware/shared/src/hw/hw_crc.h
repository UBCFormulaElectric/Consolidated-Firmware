#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_hal.h"

void     hw_crc_init(CRC_HandleTypeDef *crc_handle);
uint32_t hw_crc_calculate(uint32_t *buffer, uint32_t size);