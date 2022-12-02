#pragma once

#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedExitCode.h"

/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetDriveState(void);

/**
 * Check CAN bus for BMS Fault messages
 * @param can_rx Dim CAN RX interface for recieving messages
 * @return True if any BMS Fault is present
 */
bool App_DriveState_HasBmsFault(const struct DimCanRxInterface *can_rx);

/**
 * Check CAN bus for BMS Warning messages
 * @param can_rx Dim CAN RX interface for recieving messages
 * @return True if any BMS Warning is present
 */
bool App_DriveState_HasBmsWarning(const struct DimCanRxInterface *can_rx);
