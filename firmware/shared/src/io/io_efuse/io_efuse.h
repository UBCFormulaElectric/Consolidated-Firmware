#pragma once

#include <stdbool.h>

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

#ifndef TARGET_EMBEDDED
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
} TI_efuse;
typedef struct
{
    const Efuse *efuse1;
    const Efuse *efuse2;

    bool pgood;
    bool set_stby_reset_gpio;
} ST_efuse;
#endif

/**
 * Enable or disable the provided efuse channel.
 * @param channel Channel to enable/disable
 * @param enabled Enable if enabled is true, disable if false
 */
void io_efuse_setChannel(const Efuse *channel, bool enabled);
/**
 * Check of provided efuse channel is enabled
 * @param channel Channel to enable/disable
 * @return If efuse channel is enabled
 */
bool io_efuse_isChannelEnabled(const Efuse *channel);
/**
 * Get the current read from the provided channel
 * @param channel Channel to enable/disable
 * @return The current read from the provided channel, in A
 */
float io_efuse_getChannelCurrent(const Efuse *channel);

/**
 * @param efuse
 * @param set
 */
void io_efuse_reset_set(const ST_efuse *efuse, bool set);

/**
 * Reset the hardfault set by the efuse
 * @param efuse Reset the hardfault set by efuse
 */
void io_efuse_Reset(const ST_efuse *efuse);


/**
 * TI efuse pgood line status
 * @param efuse TI efuse in question
 * @return status of the pgood line
 */
bool io_TIefuse_pgood(const TI_efuse *efuse);