#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "App_CanTx.h"
#include "App_SharedMacros.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)
#define MAX_TS_CHARGE_CURRENT_AMPS (70.8f)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (-265.5f)

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
