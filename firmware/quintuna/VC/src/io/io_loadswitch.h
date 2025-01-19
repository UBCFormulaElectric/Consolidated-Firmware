#pragma once
 
#include <stdbool.h>

//#ifdef TARGET_EMBEDDED

#include "hw_gpios.h"
#include "hw_adcs.h"
typedef struct
{
    const Efuse *efuse1;
    const Efuse *efuse2;
    const Gpio  *stby_reset_gpio;
} LoadSwitch;
typedef struct
{
    const Gpio       *enable_gpio;
    const AdcChannel *cur_sns_adc_channel;
} Efuse;
//#else

// #include "app_utils.h"
// EMPTY_STRUCT(LoadSwitch)
// EMPTY_STRUCT(Efuse)

//#endif

extern const LoadSwitch loadswitch_front;
extern const LoadSwitch loadswitch_rear;
extern const LoadSwitch loadswitch_inverter;