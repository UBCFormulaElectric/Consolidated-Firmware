#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world);
void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world);
bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world);
void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world);
