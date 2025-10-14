#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct __EfuseFunctions EfuseFunctions;

typedef struct __TI_Efuse
{
    const Gpio *pgood;
} TI_Efuse;

extern const EfuseFunctions ti_efuse_functions;
