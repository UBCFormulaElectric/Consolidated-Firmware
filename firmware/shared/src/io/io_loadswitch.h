#pragma once

#include "io_loadswitch.h"
#include <stdbool.h>

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#include "hw_adcs.h"

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

typedef struct
{
    const Gpio       *enable_gpio;
    const AdcChannel *sns_adc_channel;
} Efuse;

typedef struct
{
    const Efuse *efuse1;
    const Efuse *efuse2;
    const Gpio  *stby_reset_gpio;
} ST_LoadSwitch;

typedef struct
{
    const Efuse *efuse;
    const Gpio  *pgood;
} TI_LoadSwitch;
#else
typedef struct
{
    bool  enabled;
    float current;
    bool  simulate_fault;
} Efuse;
typedef struct
{
    const Efuse *efuse;
    bool         pgood;
} TI_LoadSwitch;
typedef struct
{
    const Efuse *efuse1;
    const Efuse *efuse2;

    bool pgood;
    bool set_stby_reset_gpio;
} ST_LoadSwitch;
#endif

/**
 * Enable or disable the provided loadswitch channel.
 * @param channel Channel to enable/disable
 * @param enabled Enable if enabled is true, disable if false
 */
void io_loadswitch_setChannel(const Efuse *channel, bool enabled);
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
 * @param loadswitch
 * @param set
 */
void io_STloadswitch_reset_set(const ST_LoadSwitch *loadswitch, bool set);

/**
 * Reset the hardfault set by the loadswitch
 * @param loadswitch Reset the hardfault set by loadswitch
 */
void io_STloadswitch_Reset(const ST_LoadSwitch *loadswitch);

/**
 * Check if pgood is ok and if not reset the loadswitch
 * @param loadswitch Reset the hardfault set by loadswitch
 */
void io_TILoadswitch_Reset(const TI_LoadSwitch *loadSwitch);

/**
 * TI Loadswitch pgood line status
 * @param loadSwitch TI Loadswitch in question
 * @return status of the pgood line
 */
bool io_TILoadswitch_pgood(const TI_LoadSwitch *loadSwitch);