#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_DriveState.h"
#include "states/App_PreChargeState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

#include "Io_PreCharge.h"
static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct OkStatus *bms_ok_status        = App_BmsWorld_GetBmsOkStatus(world);

    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);

    App_Accumulator_InitRunOnEntry(accumulator);

    App_OkStatus_Enable(bms_ok_status);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

#include "Io_Adc.h"
#include "Io_Airs.h"
#include "Io_VoltageSense.h"
static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    float adc_v = Io_VoltageSense_GetTractiveSystemVoltage(Io_Adc_GetAdc1Channel3Voltage());
    bool is_air_neg_closed = Io_Airs_IsAirNegativeClosed();
    if (is_air_neg_closed && (adc_v < 5.0f))
    {
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetPreChargeState());
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}
