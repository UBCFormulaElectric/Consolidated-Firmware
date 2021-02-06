#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
#include "states/App_ChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_AllStates.h"

#include "App_SetPeriodicCanSignals.h"

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock       = App_BmsWorld_GetClock(world);
    struct PreCharge *pre_charge = App_BmsWorld_GetPreCharge(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);
    App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
        can_tx, CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE);

    App_PreCharge_Enable(pre_charge);
    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void
    PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct PreCharge *        pre_charge  = App_BmsWorld_GetPreCharge(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct Charger *          charger     = App_BmsWorld_GetCharger(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);
    struct Clock *            clock       = App_BmsWorld_GetClock(world);

    const uint32_t pre_charge_duration =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
        App_SharedClock_GetPreviousTimeInMilliseconds(clock);
    const float max_pack_voltage =
        App_Accumulator_GetMaxPackVoltage(accumulator);
    const float tractive_system_voltage =
        App_PreCharge_GetTractiveSystemVoltage(pre_charge);
    const struct State *next_state = App_GetPreChargeState();

    if (pre_charge_duration <= App_PreCharge_GetMinDurationMs(pre_charge))
    {
        if (tractive_system_voltage > (max_pack_voltage * 0.93f))
        {
            // Check that the tractive system voltage is less than 93% of the
            // accumulator voltage while pre-charging the inverter and capacitor
            // to 90% of the accumulator's max voltage.
            next_state = App_GetInitState();
        }
    }
    else
    {
        const float pre_charge_voltage_threshold = max_pack_voltage * 0.98f;
        if (pre_charge_duration >
            App_PreCharge_GetMinCompleteDurationMs(pre_charge))
        {
            if (tractive_system_voltage >= pre_charge_voltage_threshold)
            {
                if (App_Charger_IsConnected(charger))
                {
                    next_state = App_GetChargeState();
                }
                else if (App_SharedErrorTable_HasAnyMotorShutdownErrorSet(
                             error_table))
                {
                    next_state = App_GetDriveState();
                }
            }
            else
            {
                if (pre_charge_duration >
                    App_PreCharge_GetMaxCompleteDurationMs(pre_charge))
                {
                    App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
                        can_tx,
                        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_UNDERVOLTAGE_CHOICE);
                    next_state = App_GetInitState();
                }
            }
        }
        else
        {
            if (tractive_system_voltage > pre_charge_voltage_threshold)
            {
                App_CanTx_SetPeriodicSignal_PRE_CHARGE_FAULT(
                    can_tx,
                    CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OVERVOLTAGE_CHOICE);
                next_state = App_GetInitState();
            }
        }
    }

    App_SharedStateMachine_SetNextState(state_machine, next_state);
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
