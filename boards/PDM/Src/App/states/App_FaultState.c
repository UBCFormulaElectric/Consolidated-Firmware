
#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"

void Efuse_DisableChannels(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{
    App_Efuse_EnableChannel0(efuse1, false);
    App_Efuse_EnableChannel1(efuse1, false);
    App_Efuse_EnableChannel0(efuse2, false);
    App_Efuse_EnableChannel1(efuse2, false);
    App_Efuse_EnableChannel0(efuse3, false);
    App_Efuse_EnableChannel1(efuse3, false);
    App_Efuse_EnableChannel0(efuse4, false);
    App_Efuse_EnableChannel1(efuse4, false);
}

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse1 = App_PdmWorld_GetEfuse1(world);
    struct Efuse *   efuse2 = App_PdmWorld_GetEfuse2(world);
    struct Efuse *   efuse3 = App_PdmWorld_GetEfuse3(world);
    struct Efuse *   efuse4 = App_PdmWorld_GetEfuse4(world);

    Efuse_DisableChannels(efuse1, efuse2, efuse3, efuse4);
    App_CanTx_PDM_Vitals_State_Set(PDM_STATE_FAULT);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    if (App_CanRx_BMS_Vitals_CurrentState_Get() != BMS_FAULT_STATE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = FaultStateRunOnEntry,
        .run_on_tick_1Hz   = FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = FaultStateRunOnTick100Hz,
        .run_on_exit       = FaultStateRunOnExit,
    };

    return &fault_state;
}
