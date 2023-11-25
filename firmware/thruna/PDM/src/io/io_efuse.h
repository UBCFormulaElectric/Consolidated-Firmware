#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio       enable_gpio;
    const Gpio       stby_reset_gpio;
    const AdcChannel cur_sns_adc_channel;
} EfuseConfig;
#else
EMPTY_STRUCT(EfuseConfig);
#endif

typedef enum
{
    EFUSE_CHANNEL_AIR,
    EFUSE_CHANNEL_LVPWR,
    EFUSE_CHANNEL_EMETER,
    EFUSE_CHANNEL_AUX,
    EFUSE_CHANNEL_DRS,
    EFUSE_CHANNEL_FAN,
    EFUSE_CHANNEL_DI_LHS,
    EFUSE_CHANNEL_DI_RHS,
    NUM_EFUSE_CHANNELS,
} EfuseChannel;

/**
 * Initialize the efuses.
 * @param configs Array of configs for each efuse, with indices corresponding to EfuseChannel values.
 */
void io_efuse_init(const EfuseConfig configs[NUM_EFUSE_CHANNELS]);
#endif

/**
 * Enable or disable the provided efuse channel.
 * @param channel Channel to enable/disable
 * @param enabled Enable if enabled is true, disable if false
 */
void io_efuse_setChannel(EfuseChannel channel, bool enabled);

/**
 * Get whether or not an efuse channel is enabled.
 * @return If efuse channel is enabled
 */
bool io_efuse_isChannelEnabled(EfuseChannel channel);

/**
 * Get the current drawn by an efuse channel.
 * @param channel Channel to read current from
 * @return The current read from the provided channel, in A
 */
float io_efuse_getChannelCurrent(EfuseChannel channel);

/**
 * De-latch faults on a given efuse channel. The VND5T100LAx-E will latch itself
 * off in the case of an overtemperature or overcurrent condition. To clear these faults,
 * a low pulse must be sent on the standby reset line.
 *
 * Note that each efuse IC has 2 channels, and delatching will affect them both. We have 4 efuse ICS, with channels:
 * 1. Efuse 1: AIR and LVPWR
 * 2. Efuse 2: EMETER and AUX
 * 3. Efuse 3: DRS and FAN
 * 4. Efuse 4: DI_LHS and DI_RHS
 *
 * So be aware that, for example, a delatching faults on the AIR channel will also delatch faults on the LVPWR line.
 *
 * @param channel Channel to delatch faults for
 */
void io_efuse_standbyReset(EfuseChannel channel);