#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "App_CanTx.h"
#include "App_SharedMacros.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)
#define MAX_TS_CHARGE_CURRENT_AMPS (-70.8f)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (265.5f)

struct TractiveSystem;

/**
 * Allocate and initialize a tractive system
 * @param get_ts_raw_voltage A function that can be called to get the raw
 * voltage for the TS voltage
 * @param get_ts_voltage A function that can be called to get the tractive
 * system voltage
 */
struct TractiveSystem *App_TractiveSystem_Create(
    float (*get_ts_raw_voltage)(void),
    float (*get_ts_voltage)(float),
    float (*get_adc_high_res_current)(void),
    float (*get_high_res_current)(float),
    float (*get_adc_low_res_current)(void),
    float (*get_low_res_current)(float));

/**
 * Deallocate the memory used by the given OK status
 * @param ok_status The OK status to deallocate
 */
void App_TractiveSystem_Destroy(struct TractiveSystem *ts);

/**
 * Get the TS voltage
 * @param ts The given tractive system
 * @return The voltage in (V)
 */
float App_TractiveSystem_GetVoltage(struct TractiveSystem *ts);

/**
 * Get the TS current with high resolution voltage (40mV/A)
 * @param ts The given tractive system
 * @return The current in A
 */
float App_TractiveSystem_GetHighResCurrent(struct TractiveSystem *ts);

/**
 * Get the TS current with low resolution voltage (6.67mV/A)
 * @param ts The given tractive system
 * @return The current in A
 */
float App_TractiveSystem_GetLowResCurrent(struct TractiveSystem *ts);

/**
 * Get the TS current by choosing between the low and high resolutions
 * @param ts The given tractive system
 * @return The current in A
 */
float App_TractiveSystem_GetCurrent(struct TractiveSystem *ts);

/**
 * Get tractive system power
 * @param ts The given tractive system
 * @return TS power in Watts
 */
float App_TractiveSystem_GetPower(struct TractiveSystem *ts);

/**
 * Check the status of Tractive System faults and Warnings, sends warning over CAN bus
 * @param can_tx CAN interface to send messages over
 * @param ts The Tractive System to check faults in
 * @return True if faults present, false otherwise
 */
bool App_TractveSystem_CheckFaults(struct TractiveSystem *ts);
