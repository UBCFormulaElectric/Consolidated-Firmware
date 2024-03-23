#include <stdbool.h>
#include "io_efuse.h"
#include "app_timer.h"
#include "app_retryHandler.h"

#pragma once

typedef enum
{
    POWER_MANAGER_SHUTDOWN,
    POWER_MANAGER_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

static PowerManagerState current_power_state;

/**
 * initalizes the power manager
 */
void app_powerManager_init();

/**
 * sets the efuses for a specific PowerManagerState
 * @param state the state of the car shutdown/init or drive
 */
void app_powerManager_setState(PowerManagerState state);

/**
 * @return the current PowerManagerState
 */
PowerManagerState app_powerManager_getState();

/**
 * @note runs every 100Hz
 * @param state the current PowerManagerState's efuses being checked
 * @returns if the state machine should go to init state
 */
bool app_powerManager_check_efuses(PowerManagerState state);
