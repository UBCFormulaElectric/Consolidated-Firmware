#pragma once

#include "App_DcmWorld.h"

/**
 * Send the clear fault command to an inverter if requested by a  PCAN node
 * @param world DcmWorld struct to get the CAN Rx and CAN Tx interfaces from
 * @note Should only be called when the inverter is disabled (see pg. 38 in the
 * Software UM)
 */
void App_SendNonPeriodicCanSignals_ClearInverterFaults(
    const struct DcmWorld *world);
