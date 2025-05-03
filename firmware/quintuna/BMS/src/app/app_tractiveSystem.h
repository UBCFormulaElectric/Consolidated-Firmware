#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "app_canTx.h"
#include "app_utils.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)

// Taken from our cell's datasheet, https://www.molicel.com/wp-content/uploads/INR18650P26A-V2-80087.pdf
#define MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS (-35.0f)
#define MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS (6.0f)
#define STANDARD_TS_CHARGE_CURRENT_PER_CELL_AMPS (2.6f)

#define CELLS_IN_PARALLEL (5)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define MAX_TS_CHARGE_CURRENT_AMPS (MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define STANDARD_TS_CHARGE_CURRENT_AMPS (STANDARD_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)

#define TS_OVERCURRENT_DEBOUNCE_DURATION_MS (100U)

/**
 * Init the app tractive system module.
 */
void app_tractiveSystem_init();

/**
 * Get the TS voltage
 * @return The voltage in (V)
 */
float app_tractiveSystem_getVoltage(void);

/**
 * Get the TS current by choosing between the low and high resolutions
 * @return The current in A
 */
float app_tractiveSystem_getCurrent(void);

/**
 * Broadcast state of the tractive system over CAN.
 */
void app_tractiveSystem_broadcast(void);

/**
 * Check the status of Tractive System faults and Warnings, sends warning over CAN bus
 * @return True if faults present, false otherwise
 */
bool app_tractiveSystem_checkFaults(void);
