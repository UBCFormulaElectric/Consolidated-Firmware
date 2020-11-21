#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

bool App_FlowMetersSignals_IsPrimaryFlowRateBelowThreshold(
    struct FsmWorld *world);

bool App_FlowMetersSignals_IsPrimaryFlowRateInRange(struct FsmWorld *world);

bool App_FlowMetersSignals_IsSecondaryFlowRateBelowThreshold(
    struct FsmWorld *world);

bool App_FlowMetersSignals_IsSecondaryFlowRateInRange(struct FsmWorld *world);

void App_FlowMeterSignals_PrimaryFlowRateCallback(struct FsmWorld *world);

void App_FlowMeterSignals_SecondaryFlowRateCallback(struct FsmWorld *world);
