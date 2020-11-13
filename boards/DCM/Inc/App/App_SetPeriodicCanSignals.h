#pragma once
#include "App_DcmWorld.h"

struct DcmWorld;

/**
 * Convert pedal percentage into CAN torque request
 * @param world DcmWorld needed for canRx and canTx interface
 */
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world);

/**
 * Broadcast imu data over CAN and send InRangeCheck signals.
 * If data is invalid send a INVALID_IMU_ARGS DCM non critical errors signal.
 * @param world the DcmWorld to send the CAN signals for
 */
void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world);
