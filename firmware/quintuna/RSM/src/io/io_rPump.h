#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
typedef struct
{
    const I2cDevice *src;
} Pump;
#else
#include "app_utils.h"
EMPTY_STRUCT(Pump);
#endif

bool io_rPump_isPumpReady(const Pump *pump);
void io_rPump_writeSmth(const Pump *pump, uint8_t data);
uint8_t io_rPump_readSmth(const Pump *pump);


extern const Pump r_pump;