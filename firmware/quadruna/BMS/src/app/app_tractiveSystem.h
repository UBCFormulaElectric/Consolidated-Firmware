#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "app_canTx.h"
#include "app_utils.h"

#define HIGH_RES_MAX_CURRENT_READING (75.0f)

// Taken from our cell's datasheet on Confluence:
// https://ubcformulaelectric.atlassian.net/wiki/pages/viewpageattachments.action?pageId=55214081&preview=%2F55214081%2F55214090%2FSLPB9742126-3.7V-5900mAh-10C-V2018.pdf
#define MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS (-88.5f)
#define MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS (23.6f)

// As per datasheet, our cells can do the max discharge current for up to 3s and the peak charge current for 100ms. We
// shouldn't be anywhere near the max discharge current during driving, so we can use 100ms for charge and discharge.
#define TS_OVERCURRENT_DEBOUNCE_DURATION_MS (100U)

#define CELLS_IN_PARALLEL (3)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define MAX_TS_CHARGE_CURRENT_AMPS (MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)

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
bool app_tractveSystem_checkFaults(void);
