#pragma once

#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

typedef struct __Efuse          Efuse;
typedef struct __ST_Efuse       ST_Efuse;
typedef struct __TI_Efuse       TI_Efuse;
typedef struct __EfuseFunctions EfuseFunctions;

struct __EfuseFunctions
{
    void (*set_channel)(const Efuse *channel, bool enabled);
    bool (*is_channel_enabled)(const Efuse *channel);
    float (*get_channel_current)(const Efuse *channel);
    void (*loadswitch_reset_set)(const Efuse *channel, bool set);
    void (*reset_efuse)(const Efuse *channel);
    bool (*pgood)(const Efuse *channel);
    bool (*efuse_ok)(const Efuse *efuse);
};

struct __Efuse
{
    const Gpio       *enable_gpio;
    const AdcChannel *sns_adc_channel;
    union
    {
        ST_Efuse *st;
        TI_Efuse *ti;
    };

    const EfuseFunctions *efuse_functions;
};
