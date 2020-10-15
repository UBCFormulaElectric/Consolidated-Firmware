#pragma once

struct DcmWorld;

/**
 * Convert pedal percentage into CAN torque request
 * @param world DcmWorld needed for canRx and canTx interface
 */
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world);
