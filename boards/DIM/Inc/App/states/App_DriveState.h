#pragma once

#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedExitCode.h"

typedef enum
{
    DCM_LED,
    DIM_LED,
    FSM_LED,
    PDM_LED,
    BMS_LED,
    NUM_BOARD_LEDS,
} BoardLeds;

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
<<<<<<< HEAD
bool App_DriveState_HasBmsFault(void);
=======
bool App_DriveState_HasBmsFault(const struct DimCanRxInterface *can_rx);
>>>>>>> 972a52cc (ELEC-279 Remove BMS Faults and Warnings from Error Table (#854))

/**
 * Check CAN bus for BMS Warning messages
 * @param can_rx Dim CAN RX interface for recieving messages
 * @return True if any BMS Warning is present
 */
<<<<<<< HEAD
bool App_DriveState_HasBmsWarning(void);
=======
bool App_DriveState_HasBmsWarning(const struct DimCanRxInterface *can_rx);
>>>>>>> 972a52cc (ELEC-279 Remove BMS Faults and Warnings from Error Table (#854))
