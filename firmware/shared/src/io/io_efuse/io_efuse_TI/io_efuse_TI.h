#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_adc.h"
#include "io_efuse/io_efuse_datatypes.h"

typedef struct __TI_Efuse
{
    const Gpio *pgood;
} TI_Efuse;

extern const EfuseFunctions ti_efuse_functions;
