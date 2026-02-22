#pragma once

#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

#include "util_errorCodes.h"

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

typedef struct __Efuse          Efuse;
typedef struct __ST_VND5_Efuse  ST_VND5_Efuse;
typedef struct __TI_TPS25_Efuse TI_TPS25_Efuse;
typedef struct __TI_TPS28_Efuse TI_TPS28_Efuse;
typedef struct __EfuseFunctions EfuseFunctions;

struct __EfuseFunctions
{
    void (*set_channel)(const Efuse *channel, bool enabled);
    bool (*is_channel_enabled)(const Efuse *channel);
    float (*get_channel_current)(const Efuse *channel);
    void (*loadswitch_reset_set)(const Efuse *channel, bool set);
    void (*reset_efuse)(const Efuse *channel);
    bool (*pgood)(const Efuse *channel);
    ExitCode (*efuse_ok)(const Efuse *efuse);
    bool (*set_diagnostics)(const Efuse *efuse, bool enabled);
};

struct __Efuse
{
    const Gpio       *enable_gpio;
    const AdcChannel *sns_adc_channel;
    union
    {
        ST_VND5_Efuse  *st_vnd5;
        TI_TPS25_Efuse *ti_tps25;
        TI_TPS28_Efuse *ti_tps28;
    };

    const EfuseFunctions *efuse_functions;
};
