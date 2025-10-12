#pragma once

#include "hw_gpio.h"
#include "hw_adc.h"
#include "io_efuse_ST/io_efuse_ST.H"
#include "io_efuse_TI/io_efuse_TI.h"

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

typedef struct __Efuse    Efuse;
typedef struct __ST_Efuse ST_Efuse;
typedef struct __TI_Efuse TI_Efuse;

typedef struct
{
    void (*set_channel)(Efuse *channel, bool enabled);
    bool (*is_channel_enabled)(const Efuse *channel);
    bool (*get_channel_current)(const Efuse *channel);
    void (*loadswitch_reset_set)(const Efuse *channel, const bool set);
    void (*reset_efuse)(const Efuse *channel);

} EfuseFunctions;

typedef struct __Efuse
{
    const Gpio       *enable_gpio;
    const AdcChannel *sns_adc_channel;
    union
    {
        ST_Efuse *st;
        TI_Efuse *ti;
    };

    EfuseFunctions *efuse_functions;
} Efuse;
