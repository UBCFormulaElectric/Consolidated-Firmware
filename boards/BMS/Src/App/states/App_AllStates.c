#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(can_tx, charger_is_connected);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx  = App_BmsWorld_GetCanTx(world);
    struct OkStatus *         bms_ok  = App_BmsWorld_GetBmsOk(world);
    struct OkStatus *         imd_ok  = App_BmsWorld_GetImdOk(world);
    struct OkStatus *         bspd_ok = App_BmsWorld_GetBspdOk(world);

    if (App_OkStatus_IsEnabled(bms_ok))
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(imd_ok))
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(bspd_ok))
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, false);
    }
}
