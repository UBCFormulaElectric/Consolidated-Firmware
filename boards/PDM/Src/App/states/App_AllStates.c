#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence =
        App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *  can_tx = App_PdmWorld_GetCanTx(world);
    struct LowVoltageBatteries *low_voltage_batteries =
        App_PdmWorld_GetLowVoltageBatteries(world);

    if (App_LowVoltageBatteries_IsOvervoltage(low_voltage_batteries))
    {
        App_CanTx_SetPeriodicSignal_CELL_BALANCE_OVERVOLTAGE_FAULT(
            can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_CELL_BALANCE_OVERVOLTAGE_FAULT(
            can_tx, false);
    }

    if (App_LowVoltageBatteries_HasChargeFault(low_voltage_batteries))
    {
        App_CanTx_SetPeriodicSignal_CHARGER_FAULT(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_CHARGER_FAULT(can_tx, false);
    }

    if (App_LowVoltageBatteries_HasBoostControllerFault(low_voltage_batteries))
    {
        App_CanTx_SetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx, false);
    }
}
