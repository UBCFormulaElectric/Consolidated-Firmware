#pragma once

#include "App_FsmWorld.h"

void App_SetPeriodicSignals_FlowRateInRangeChecks(const struct FsmWorld *world);
void App_SetPeriodicSignals_WheelSpeedInRangeChecks(
    const struct FsmWorld *world);
void App_SetPeriodicSignals_SteeringAngleInRangeCheck(
    const struct FsmWorld *world);
void App_SetPeriodicSignals_BrakePressureInRangeCheck(
    const struct FsmWorld *world);
void App_SetPeriodicSignals_BrakeActuationStatus(const struct FsmWorld *world);
void App_SetPeriodicSignals_BrakeOpenOrShortCircuitedStatus(
    const struct FsmWorld *world);
