#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_CanRx.h"
#include "App_CanTx.h"

typedef struct RegenBraking
{
    float left_inverter_torque;
    float right_inverter_torque;
} RegenBraking;

/**
 * Run safety checks before starting regenerative braking and execute
 * regen if satisfied
 */
void App_Run_Regen(void);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @return true or false depending on if it is safe to run regen
 */
bool App_Regen_Safety(struct RegenBraking *regenAttr);

/**
 * Activate torque requests for regen
 * @param left is the left inverter torque request
 * @param right is the right inverter torque request
 */
void App_Regen_Activate(float left, float right);
