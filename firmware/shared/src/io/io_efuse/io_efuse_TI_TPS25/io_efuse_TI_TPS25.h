#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct __EfuseFunctions EfuseFunctions;

typedef struct __TI_TPS25_Efuse
{
    const Gpio *pgood;
} TI_TPS25_Efuse;

extern const EfuseFunctions TI_TPS25_Efuse_functions;
