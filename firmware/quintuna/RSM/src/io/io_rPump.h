#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
#include "hw_i2c.h"

bool io_rPump_isPumpReady(const I2cDevice *device);
void io_rPump_write(const I2cDevice *device, uint8_t data);
uint8_t io_rPump_read(const I2cDevice *device);
#endif