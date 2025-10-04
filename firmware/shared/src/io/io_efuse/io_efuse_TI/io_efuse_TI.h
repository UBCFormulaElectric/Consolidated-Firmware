#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_adc.h"
#include "io_efuse_datatypes.h"


/* Forward declarations for types you use here */
typedef EfuseFunctons EfuseFunctons;

typedef struct ST_Efuse
{
    const Gpio       *pgood;
}TI_Efuse;


extern const EfuseFunctons ti_efuse_functions;

