#pragma once
#include "App_DcmWorld.h"

struct DcmWorld;

/**
 * Convert pedal percentage into CAN torque request
 * @param world DcmWorld needed for canRx and canTx interface
 */
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world);

/**
 * Broadcast imu data
 * @param world current DcmWorld
 */
void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world);
