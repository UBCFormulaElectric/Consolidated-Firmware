#include "App_Regen.h"

static bool wheel_speed_in_range(void);
static bool power_limit_check(void);
static void compute(struct RegenBraking *regenAttr);

struct RegenBraking regenAttributes;
static float MAXREGEN = -50.0;
static float wheelSpeedThreshold = 5.0;

void App_Run_Regen(void) {
    if (App_Regen_Safety(&regenAttributes)) {

        App_Regen_Activate(regenAttributes.left_inverter_torque, regenAttributes.right_inverter_torque);

    } else {

        App_Regen_Deactivate(regenAttributes.left_inverter_torque, regenAttributes.right_inverter_torque);

    }
}

bool App_Regen_Safety(struct RegenBraking *regenAttr)
{
    if (App_CanRx_BMS_CellTemperatures_MaxCellTemperature_Get() < 45
    && wheel_speed_in_range() 
    && power_limit_check()) {
        compute(regenAttr);
    }

    regenAttr->left_inverter_torque = 0.0;
    regenAttr->right_inverter_torque = 0.0;

    return true;
}

void App_Regen_Activate(float left, float right) {
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(right);
}

void App_Regen_Deactivate(float left, float right) {
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(right);
}

static bool wheel_speed_in_range(void) {
    float right =  App_CanRx_FSM_Wheels_RightWheelSpeed_Get();
    float left = App_CanRx_FSM_Wheels_LeftWheelSpeed_Get();
    if (right > wheelSpeedThreshold && left > wheelSpeedThreshold) {
        return true;
    }

    return false;
}

static bool power_limit_check(void) {
    //TODO: Update check once power limiting is available @Will Chaba
    if (App_CanRx_BMS_CellVoltages_MaxCellVoltage_Get() < 4.0f) {
        return true;
    }

    return false;
}

static void compute(struct RegenBraking *regenAttr) {
    float accelerator_pedal_percentage = App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get();

    float torque = accelerator_pedal_percentage <= 50.0f ? -(50.0f - accelerator_pedal_percentage)/50.0f*MAXREGEN : 0.0f;

    regenAttr->left_inverter_torque = torque;
    regenAttr->right_inverter_torque = torque;
}