#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "Io_OkStatuses.h"
#include "Io_Airs.h"
#include "states/App_PreChargeState.h"
#include "Io_VoltageSense.h"

#include "Io_LTC6813.h"
static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct Clock *            clock       = App_BmsWorld_GetClock(world);

    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    App_Accumulator_InitRunOnEntry(accumulator);

    // Io_LTC6813_SetCfgRegsToDefaultSettings();
    // Io_LTC6813_SetCfgRegsToDefaultSettings();
    // Io_LTC6813_ConfigureRegisterB();

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);

    // BMS is running properly
    Io_OkStatuses_EnableBmsOk();
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    // if AIR- closes and the TS voltage is below 5V then begin precharge
    if (Io_Airs_IsAirNegativeClosed() &&
        Io_VoltageSense_GetTractiveSystemVoltage() < 5)
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
