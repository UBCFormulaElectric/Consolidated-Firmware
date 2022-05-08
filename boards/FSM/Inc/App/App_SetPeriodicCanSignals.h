#pragma once

#include "App_FsmWorld.h"

void App_SetPeriodicSignals_FlowRateInRangeChecks(const struct FsmWorld *world);
void App_SetPeriodicSignals_WheelSpeedInRangeChecks(const struct FsmWorld *world);
void App_SetPeriodicSignals_SteeringAngleInRangeCheck(const struct FsmWorld *world);
void App_SetPeriodicSignals_Brake(const struct FsmWorld *world);
void App_SetPeriodicSignals_AcceleratorPedal(const struct FsmWorld *world);
void App_SetPeriodicSignals_MotorShutdownFaults(const struct FsmWorld *world);
