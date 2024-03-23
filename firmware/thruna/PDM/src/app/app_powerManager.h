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

void app_powerManager_init();

void app_powerManager_setState(PowerManagerState state);

PowerManagerState app_powerManager_getState();

bool app_powerManager_check_efuses(PowerManagerState state);
