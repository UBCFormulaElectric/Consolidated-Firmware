#pragma once

#include "app_stateMachine.h"

typedef enum
{
    BMS_LED,
    FSM_LED,
    VC_LED,
    AUX_LED,
    CRIT_LED,
    RSM_LED,
    NUM_BOARD_LEDS,
} BoardLeds;

/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State.
 */
const State *app_mainState_get(void);
