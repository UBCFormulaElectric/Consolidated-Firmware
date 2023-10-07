#include "App_Regen.h"

bool wheel_speed_in_range(void);
struct RegenBraking regenAttributes;

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
    if (App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get() == 0 
    && App_CanRx_BMS_CellTemperatures_MaxCellTemperature_Get() < 45
    && wheel_speed_in_range()) {
        regenAttr->left_inverter_torque = -1.0;
        regenAttr->right_inverter_torque = -1.0;
    }

    regenAttr->left_inverter_torque = 0.0;
    regenAttr->right_inverter_torque = 0.0;
    // check if wheel speed is between 0 and 5km
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

bool wheel_speed_in_range(void) {
    float right =  App_CanRx_FSM_Wheels_RightWheelSpeed_Get();
    float left = App_CanRx_FSM_Wheels_LeftWheelSpeed_Get();
    if (right > wheelSpeedThreshold && left > wheelSpeedThreshold) {
        return true;
    }

    return false;
}