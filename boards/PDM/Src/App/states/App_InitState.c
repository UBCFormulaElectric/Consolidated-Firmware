#include <math.h>

#include "states/App_AllStates.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx_interface = App_PdmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_PDM_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct PdmWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);

    App_SetPeriodicCanSignals_VoltageInRangeChecks(world);

    // The e-fuse watchdog may have timed out due to the boot-up delay, so we
    // will assume that there is no current to be read in this state.
    App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_CAN_CURRENT(can_tx, NAN);
    App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT(can_tx, NAN);
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState()
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
