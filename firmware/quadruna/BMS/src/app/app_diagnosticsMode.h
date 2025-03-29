#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "ltc6813/io_ltc6813Shared.h"

/**
 * Broadcast state of the accumulator diagnostics over CAN.
 */
void app_diagnosticsMode_broadcast(void);

/**
 * Set all cell voltages and segment temperatures to negative values
 */
void app_diagnosticsMode_invalidateValues(void);