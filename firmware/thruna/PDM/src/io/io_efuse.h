#pragma once

#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio enable_gpio;
    const Gpio stby_reset_gpio;
    const AdcChannel isense_adc_channel;
} EfuseConfig;

typedef struct 
{
    bool enabled;
    const EfuseConfig* config;
} EFuse;
 
 /**
 * Initialize an efuse.
 * @param efuse EFuse to init.
 * @param config Config params.
 */
void io_efuse_init(EFuse* efuse, const EfuseConfig* config);

/**
 * Enable or disable the provided efuse.
 * @param efuse Efuse to enable/disable.
 * @param enabled Enable if enabled is true, disable if false.
 */
void io_efuse_setChannel(EFuse* efuse, bool enabled);

/**
 * Get whether or not an efuse is enabled.
 * @return If efuse is enabled.
 */
bool io_efuse_isChannelEnabled(EFuse* efuse);

/**
 * Get the current drawn by an efuse.
 * @param efuse Efuse to read current from
 * @return The current read from the provided efuse, in A.
 */
float io_efuse_getChannelCurrent(EFuse* efuse);

/**
 * De-latch faults on a given efuse. The VND5T100LAx-E will latch itself
 * off in the case of an overtemperature or overcurrent condition. To clear these faults,
 * a low pulse must be sent on the standby reset line.
 *
 * Note that each IC has 2 seperate efuses, and delatching will affect them both. We have 4 ICS, with efuses:
 * 1. IC 1: AIR and LVPWR
 * 2. IC 2: EMETER and AUX
 * 3. IC 3: DRS and FAN
 * 4. IC 4: DI_LHS and DI_RHS
 *
 * So be aware that, for example, a delatching faults on the AIR channel will also delatch faults on the LVPWR line.
 *
 * @param efuse Channel to delatch faults for.
 */
void io_efuse_standbyReset(EFuse* efuse);