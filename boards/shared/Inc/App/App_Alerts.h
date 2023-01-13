#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_AlertsConfig.h"

void App_Alerts_Init(void);

void App_Alerts_Set(Board_Alert id, bool set);

void App_Alerts_Push(Board_Alert id);
