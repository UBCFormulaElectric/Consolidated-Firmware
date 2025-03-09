#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#include "hw_adcs.h"
typedef struct
{
    const Gpio       *enable_gpio;
    const AdcChannel *cur_sns_adc_channel;
} Efuse;
typedef struct
{
    const Efuse *efuse1;
    const Efuse *efuse2;
    const Gpio  *stby_reset_gpio;
} LoadSwitch;
#else

#include "app_utils.h"
EMPTY_STRUCT(LoadSwitch)
EMPTY_STRUCT(Efuse)

#endif

extern const LoadSwitch loadswitch_front;
extern const LoadSwitch loadswitch_rear;
extern const LoadSwitch loadswitch_inverter;

/**
 * Enable or disable the provided loadswitch channel.
 * @param channel Channel to enable/disable
 * @param enabled Enable if enabled is true, disable if false
 */
void io_loadswitch_setChannel(const Efuse *channel, const bool enabled);
/**
 * Check of provided loadswitch channel is enabled
 * @param channel Channel to enable/disable
 * @return If loadswitch channel is enabled
 */
bool io_loadswitch_isChannelEnabled(const Efuse *channel);
/**
 * Get the current read from the provided channel
 * @param channel Channel to enable/disable
 * @return The current read from the provided channel, in A
 */
float io_loadswitch_getChannelCurrent(const Efuse *channel);
/**
 * Reset the hardfault set by the loadswitch
 * @param loadswitch Reset the hardfault set by loadswitch
 */
void io_loadswitch_standbyReset(const LoadSwitch *loadswitch);