#include "states/App_AllStates.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"
#include "states/App_FaultState.h"
#include "states/App_DriveState.h"

void Efuse_Enable_Channels_18650Startup(
    struct Efuse *efuse1,
    struct Efuse *efuse2,
    struct Efuse *efuse3,
    struct Efuse *efuse4)
{
    App_Efuse_EnableChannel0(efuse1);
    App_Efuse_EnableChannel1(efuse1);
    App_Efuse_EnableChannel0(efuse2);
    App_Efuse_DisableChannel1(efuse2);
    App_Efuse_EnableChannel0(efuse3);
    App_Efuse_EnableChannel1(efuse3);
    App_Efuse_DisableChannel0(efuse4);
    App_Efuse_DisableChannel1(efuse4);
}

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse1 = App_PdmWorld_GetEfuse1(world);
    struct Efuse *   efuse2 = App_PdmWorld_GetEfuse2(world);
    struct Efuse *   efuse3 = App_PdmWorld_GetEfuse3(world);
    struct Efuse *   efuse4 = App_PdmWorld_GetEfuse4(world);

    Efuse_Enable_Channels_18650Startup(efuse1, efuse2, efuse3, efuse4);
}

void Efuse_ErrorsWarnings_CANTX(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{
    App_CanTx_PDM_EfuseFaultCheck_AIR_Set(App_Efuse_FaultCheckChannel0(efuse1));
    App_CanTx_PDM_EfuseFaultCheck_LVPWR_Set(App_Efuse_FaultCheckChannel1(efuse1));
    App_CanTx_PDM_EfuseFaultCheck_EMETER_Set(App_Efuse_FaultCheckChannel0(efuse2));
    App_CanTx_PDM_EfuseFaultCheck_AUX_Set(App_Efuse_FaultCheckChannel1(efuse2));
    App_CanTx_PDM_EfuseFaultCheck_LEFT_INVERTER_Set(App_Efuse_FaultCheckChannel0(efuse3));
    App_CanTx_PDM_EfuseFaultCheck_RIGHT_INVERTER_Set(App_Efuse_FaultCheckChannel1(efuse3));
    App_CanTx_PDM_EfuseFaultCheck_DRS_Set(App_Efuse_FaultCheckChannel0(efuse4));
    App_CanTx_PDM_EfuseFaultCheck_FAN_Set(App_Efuse_FaultCheckChannel1(efuse4));
}

bool InitFaultDetection(
    struct Efuse *         efuse1,
    struct Efuse *         efuse2,
    struct Efuse *         efuse3,
    struct Efuse *         efuse4,
    struct RailMonitoring *rail_monitor)
{
    if (App_Efuse_FaultCheckChannel0(efuse1) == false && // AIR
        App_Efuse_FaultCheckChannel0(efuse2) == false && // EMETER
        App_Efuse_FaultCheckChannel0(efuse3) == false && // LEFT INVERTER
        App_Efuse_FaultCheckChannel1(efuse3) == false && // RIGHT INVERTER
        !App_RailMonitoring_VbatVoltageLowCheck(rail_monitor) &&
        !App_RailMonitoring_24VAccumulatorVoltageLowCheck(rail_monitor) &&
        !App_RailMonitoring_22VAuxiliaryVoltageLowCheck(rail_monitor))
        return false; // No Error
    return true;      // Error
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct PdmWorld *      world        = App_SharedStateMachine_GetWorld(state_machine);
    struct RailMonitoring *rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *         efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *         efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *         efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *         efuse4       = App_PdmWorld_GetEfuse4(world);
    bool                   has_fault;

    has_fault = InitFaultDetection(efuse1, efuse2, efuse3, efuse4, rail_monitor);
    Efuse_ErrorsWarnings_CANTX(efuse1, efuse2, efuse3, efuse4);

    if (has_fault)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
    else if (App_CanRx_BMS_Vitals_CurrentState_Get() == BMS_DRIVE_STATE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
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
