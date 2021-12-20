#include <Io_CellTemperatures.h>
#include <Io_CellVoltages.h>
#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
#include "states/App_ChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_AllStates.h"

#include "App_SetPeriodicCanSignals.h"

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx     = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock      = App_BmsWorld_GetClock(world);
    struct PreCharge *        pre_charge = App_BmsWorld_GetPreCharge(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);
    App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
        can_tx, CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE);

    App_PreCharge_Enable(pre_charge);
    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    // If you want to read cell and temp voltages uncomment lines below
    //    Io_CellTemperatures_ReadTemperatures();
    //    Io_CellVoltages_ReadRawCellVoltages();
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static float _getPrechargeVoltageThreshold(const float max_pack_voltage)
{
    // todo: maybe convert to constant?
    return max_pack_voltage * 0.98f;
}

static void _prechargeUnderMinDurationRunOnTick100Hz(
    struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct PreCharge *        pre_charge  = App_BmsWorld_GetPreCharge(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);

    const float max_pack_voltage =
        App_Accumulator_GetMaxPackVoltage(accumulator);
    const float tractive_system_voltage =
        App_PreCharge_GetTractiveSystemVoltage(pre_charge);
    const float pre_charge_voltage_threshold =
        _getPrechargeVoltageThreshold(max_pack_voltage);

    // Check for overvoltage
    if (tractive_system_voltage > pre_charge_voltage_threshold * 0.93f)
    {
        // todo: ask Prom about 0.93f code.
        App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
            can_tx,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OVERVOLTAGE_CHOICE);
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        return;
    }
    else
    {
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetPreChargeState());
    }
}

static void _prechargeOverMaxDurationRunOnTick100Hz(
    struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);

    // Precharge failure
    // BMS-43, BMS-46
    App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
        can_tx,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_UNDERVOLTAGE_CHOICE);
    // todo: BMS-46: reset the AIR shutdown latch when AIR- is opened

    // todo: ask why we enter init state instead of fault state in case of
    // precharge failure
    App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
}

static void
    _prechargeInDurationRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world = App_SharedStateMachine_GetWorld(state_machine);
    struct PreCharge *  pre_charge  = App_BmsWorld_GetPreCharge(world);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct Charger *    charger     = App_BmsWorld_GetCharger(world);
    struct Airs *       airs        = App_BmsWorld_GetAirs(world);

    const float max_pack_voltage =
        App_Accumulator_GetMaxPackVoltage(accumulator);
    const float tractive_system_voltage =
        App_PreCharge_GetTractiveSystemVoltage(pre_charge);

    const float pre_charge_voltage_threshold = max_pack_voltage * 0.98f;

    // Precharge successful
    if (tractive_system_voltage >= pre_charge_voltage_threshold)
    {
        // BMS-42: Close AIR+ after successful precharge
        App_Airs_CloseAirPositive(airs);

        // BMS-44: Precharge success with charger
        if (App_Charger_IsConnected(charger))
        {
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetChargeState());
            return;
        }

        // todo: add ErrorTable_HasAnyMotorShutdownErrorSet checks to drive
        // state BMS-45: Precharge success without charger
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
    else
    {
        // Keep precharging
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetPreChargeState());
    }
}

static void
    PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld * world = App_SharedStateMachine_GetWorld(state_machine);
    struct PreCharge *pre_charge = App_BmsWorld_GetPreCharge(world);
    struct Clock *    clock      = App_BmsWorld_GetClock(world);

    const uint32_t pre_charge_duration =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
        App_SharedClock_GetPreviousTimeInMilliseconds(clock);
    if (pre_charge_duration <= App_PreCharge_GetMinDurationMs(pre_charge))
    {
        _prechargeUnderMinDurationRunOnTick100Hz(state_machine);
    }
    else if (
        pre_charge_duration >
        App_PreCharge_GetMaxCompleteDurationMs(pre_charge))
    {
        _prechargeOverMaxDurationRunOnTick100Hz(state_machine);
    }
    else
    {
        _prechargeInDurationRunOnTick100Hz(state_machine);
    }
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld * world = App_SharedStateMachine_GetWorld(state_machine);
    struct PreCharge *pre_charge = App_BmsWorld_GetPreCharge(world);

    App_PreCharge_Disable(pre_charge);
}

const struct State *App_GetPreChargeState(void)
{
    static struct State pre_charge_state = {
        .name              = "PRE_CHARGE",
        .run_on_entry      = PreChargeStateRunOnEntry,
        .run_on_tick_1Hz   = PreChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = PreChargeStateRunOnTick100Hz,
        .run_on_exit       = PreChargeStateRunOnExit,
    };

    return &pre_charge_state;
}
