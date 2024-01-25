#pragma once

#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"

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
 * @return A pointer to the Drive State.
 */
const struct State * app_driveState_get(void);
