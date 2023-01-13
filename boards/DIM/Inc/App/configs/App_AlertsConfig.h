#pragma once

#include <stdint.h>
#include "App_CanUtils.h"

#define Board_Alert DIM_Alert
#define NUM_BOARD_ALERTS NUM_DIM_ALERT_CHOICES

void App_AlertsConfig_SendAlertsSet(uint32_t data);
void App_AlertsConfig_SendAlertsCleared(uint32_t data);
