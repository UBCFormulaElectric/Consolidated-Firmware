#pragma once

#include <assert.h>
#include <stdbool.h>

#define TS_OVERCURRENT_DEBOUNCE_DURATION_MS (100U)

/**
 * Init the app tractive system module.
 */
void app_tractiveSystem_init(void);

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
