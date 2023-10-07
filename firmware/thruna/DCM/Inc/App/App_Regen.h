#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_CanRx.h"
#include "App_CanTx.h"

struct RegenBraking
{
    float vehicle_speed_kph;
    float left_inverter_torque;
    float right_inverter_torque;
};
void App_Run_Regen(void);
bool App_Regen_Safety(struct RegenBraking *regenAttr);
void App_Regen_Activate(float left, float right);
void App_Regen_Deactivate(float left, float right);