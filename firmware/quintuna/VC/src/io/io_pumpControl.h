#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_i2c.h"
typedef struct {
    I2cDevice i2cDevice;
} PumpControl;
#else
#endif