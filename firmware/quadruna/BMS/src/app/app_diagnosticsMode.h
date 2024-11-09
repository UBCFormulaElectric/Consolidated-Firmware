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
 * Obtain and store the voltage stats of cells in a given segment in the accumulator.
 */
void app_diagnosticsMode_calculateDiagnosticVoltageStats(uint8_t seg_requested);

/**
 * Calculate the average temperature of each segment.
 */
void app_diagnosticsMode_calculateDiagnosticTemperatureStats(void);