#pragma once

#include <stdbool.h>
#include "app_timer.h"
#include "app_powerManager.h"

void app_powerSequencing_init(void);
bool app_powerSequencing_run(void);
bool app_powerSequencing_checkAllLoads(PowerState power_state);