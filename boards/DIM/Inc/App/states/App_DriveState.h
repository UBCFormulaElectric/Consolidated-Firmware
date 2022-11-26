#pragma once

#include "App_SharedStateMachine.h"

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
const bool has_bms_fault(const struct DimCanRxInterface *can_rx);

/**
 * Check CAN bus for BMS Warning messages
 * @param can_rx Dim CAN RX interface for recieving messages
 * @return True if any BMS Warning is present
 */
const bool has_bms_warning(const struct DimCanRxInterface *can_rx);
