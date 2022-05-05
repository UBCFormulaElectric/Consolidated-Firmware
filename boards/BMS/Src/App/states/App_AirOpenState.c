#include "states/App_AllStates.h"
#include "states/App_PreChargeState.h"

#include "App_SharedMacros.h"

static void AirOpenStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_AIR_OPEN_CHOICE);
}

static void AirOpenStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void AirOpenStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Airs *    airs  = App_BmsWorld_GetAirs(world);

    // Begin the precharge sequence if AIR- is closed
    if (App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetPreChargeState());
    }
}

static void AirOpenStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirOpenState(void)
{
    static struct State air_open_state = { .name              = "AIR_OPEN",
                                           .run_on_entry      = AirOpenStateRunOnEntry,
                                           .run_on_tick_1Hz   = AirOpenStateRunOnTick1Hz,
                                           .run_on_tick_100Hz = AirOpenStateRunOnTick100Hz,
                                           .run_on_exit       = AirOpenStateRunOnExit };

    return &air_open_state;
}
