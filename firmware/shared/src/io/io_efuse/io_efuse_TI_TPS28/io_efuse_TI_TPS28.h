#pragma once

#include "hw_gpio.h"

typedef struct __EfuseFunctions EfuseFunctions;

typedef struct __TI_TPS28_Efuse
{
    const Gpio *fault_gpio;
    const Gpio *diag_en_gpio;

    /* Portable bit-fields: use 'unsigned' and name the subgroup */
    union
    {
        struct
        {
            uint8_t overcurrent : 1;
            uint8_t thermal_shdn : 1;
            uint8_t padding : 6;
        } flags;
        uint8_t raw;
    } faults;
} TI_TPS28_Efuse;

extern const EfuseFunctions TI_TPS28_EfuseFunctions;