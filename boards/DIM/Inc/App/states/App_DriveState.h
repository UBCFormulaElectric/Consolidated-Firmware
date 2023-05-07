#pragma once

#include "App_SevenSegDisplays.h"
#include "App_SharedExitCode.h"
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"

typedef enum {
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
