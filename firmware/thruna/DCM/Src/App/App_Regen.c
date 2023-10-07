#include "App_Regen.h"

struct RegenBraking regenAttributes;

void App_Run_Regen(void) {
    if (App_Regen_Safety(&regenAttributes)) {

        App_Regen_Activate(&regenAttributes);

    } else {

        App_Regen_Deactivate(&regenAttributes);

    }
}

bool App_Regen_Safety(struct RegenBraking *regenAttr)
{
    if (App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get() == 0 && App_CanRx_BMS_CellTemperatures_MaxCellTemperature_Get() < 45) {
        regenAttr->left_inverter_torque = -1.0;
        regenAttr->right_inverter_torque = -1.0;
    }

    regenAttr->left_inverter_torque = 0.0;
    regenAttr->right_inverter_torque = 0.0;
    // check if wheel speed is between 0 and 5km
    return true;
}

void App_Regen_Activate(struct RegenBraking *regenAttr) {
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(regenAttr->left_inverter_torque);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(regenAttr->right_inverter_torque);
}

void App_Regen_Deactivate(struct RegenBraking *regenAttr) {
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(regenAttr->left_inverter_torque);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(regenAttr->right_inverter_torque);
}