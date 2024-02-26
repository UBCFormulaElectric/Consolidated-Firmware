#include <stdbool.h>
#include "io_efuse.h"
#include "app_timer.h"

#pragma once

#define FAULT_CURRENT_THRESHOLD 0.5f
#define CHECK_TIME 200

typedef enum
{
    POWER_MANAGER_SHUTDOWN,
    POWER_MANAGER_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

static PowerManagerState current_power_state = POWER_MANAGER_SHUTDOWN;

void app_powerManager_setState(PowerManagerState state);

PowerManagerState app_powerManager_getState();
