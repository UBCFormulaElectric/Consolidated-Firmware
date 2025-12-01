#pragma once

#include <stdbool.h>
#include "io_efuse_datatypes.h"
// #include "io_efuse_ST/io_efuse_ST.h"
// #include "io_efuse_TI/io_efuse_TI.h"

typedef struct __Efuse Efuse;

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
void io_efuse_reset_set(const Efuse *channel, bool set);

/**
 * Reset the hardfault set by the efuse
 * @param efuse Reset the hardfault set by efuse
 */
void io_efuse_reset(const Efuse *channel);

/**
 * @brief Check the pgood status
 * @param channel: TI Efuse
 * @return status of the pgood line
 */
bool io_efuse_pgood(const Efuse *channel);

/**
 * @brief Check the if the efuse status is ok
 * @param efuse: efuse to check status on
 * @return True if status is ok, False otherwise
 */
bool io_efuse_ok(const Efuse *efuse);

/**
 * Enable or disable the provided efuse diagnostics.
 * @param efuse Channel to enable/disable
 * @param enabled Enable if enabled is true, disable if false
 */
void io_efuse_setDiagnostics(const Efuse *efuse, bool enabled);