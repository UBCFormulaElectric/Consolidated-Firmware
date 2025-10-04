#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_adc.h"
#include "io_efuse/io_efuse_datatypes.h"

typedef struct __ST_Efuse
{
    const Gpio *stby_reset_gpio;

    /* Portable bit-fields: use 'unsigned' and name the subgroup */
    struct
    {
        uint8_t overload : 1;
        uint8_t ovt_stp : 1;
        uint8_t under_voltage : 1;
        uint8_t short_to_vbat : 1;
        uint8_t open_load_off_stat : 1;
        uint8_t negative_output_voltage_clamp : 1;
    } faults;
} ST_Efuse;

extern const EfuseFunctons st_efuse_functions;
