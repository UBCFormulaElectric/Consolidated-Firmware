#include "states/App_AllStates.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
}

static void ChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);

    // struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    // struct BmsCanTxInterface * can_tx = App_BmsWorld_GetCanTx(world);
    // const struct CellMonitors *cell_monitors =
    //    App_BmsWorld_GetCellMonitors(world);
    // struct Charger *charger = App_BmsWorld_GetCharger(world);

    // App_CellMonitors_ReadDieTemps(cell_monitors);
    // App_SetPeriodicSignals_CellMonitorsInRangeChecks(can_tx, cell_monitors);

    // float                 max_die_temperature;
    // enum ITMPInRangeCheck cell_monitor_itmp_in_range_check =
    //    App_CellMonitors_GetMaxDieTempDegC(cell_monitors,
    //    &max_die_temperature);
    // App_CanTx_SetPeriodicSignal_MAX_CELL_MONITOR_DIE_TEMPERATURE(
    //    can_tx, max_die_temperature);

    // if (cell_monitor_itmp_in_range_check == ITMP_OVERFLOW)
    //{
    //    if (App_Charger_IsEnabled(charger))
    //    {
    //        App_Charger_Disable(charger);
    //    }

    //    App_CanTx_SetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
    //        can_tx,
    //        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_TRUE_CHOICE);
    //}
    // else if (cell_monitor_itmp_in_range_check == ITMP_CHARGER_IN_RANGE)
    //{
    //    if (!App_Charger_IsEnabled(charger))
    //    {
    //        App_Charger_Enable(charger);
    //    }

    //    App_CanTx_SetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
    //        can_tx,
    //        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_FALSE_CHOICE);
    //}
}

static void ChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx  = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger = App_BmsWorld_GetCharger(world);

    if (!App_Charger_IsConnected(charger))
    {
        App_CanTx_SetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(
            can_tx, true);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetChargeState(void)
{
    static struct State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = ChargeStateRunOnEntry,
        .run_on_tick_1Hz   = ChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = ChargeStateRunOnTick100Hz,
        .run_on_exit       = ChargeStateRunOnExit,
    };

    return &charge_state;
}
