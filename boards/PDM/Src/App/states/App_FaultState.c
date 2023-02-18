
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_InitState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_FaultState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *const world = App_SharedStateMachine_GetWorld(state_machine);
}

bool Faults(
    struct Efuse *         efuse1,
    struct Efuse *         efuse2,
    struct Efuse *         efuse3,
    struct Efuse *         efuse4,
    struct RailMonitoring *rail_monitor)
{
    int num_fixed = 0;
    if (App_Efuse_FaultProcedure_Channel0(efuse1, 3) == 0)
    {
        // AIR
        App_CanTx_PDM_Efuse_Fault_Checks_AIR_Set(false);
        num_fixed++;
    }
    if (App_Efuse_FaultProcedure_Channel0(efuse2, 0) == 0)
    {
        // EMETER
        App_CanTx_PDM_Efuse_Fault_Checks_EMETER_Set(false);
        num_fixed++;
    }
    if (App_Efuse_FaultProcedure_Channel0(efuse3, 0) == 0)
    {
        // LEFT INVERTER
        App_CanTx_PDM_Efuse_Fault_Checks_LEFT_INVERTER_Set(false);
        num_fixed++;
    }
    if (App_Efuse_FaultProcedure_Channel1(efuse3, 0) == 0)
    {
        // LEFT INVERTER
        App_CanTx_PDM_Efuse_Fault_Checks_RIGHT_INVERTER_Set(false);
        num_fixed++;
    }
    if (!App_RailMonitoring_VBAT_VoltageCriticalCheck(rail_monitor))
    {
        // VBAT
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        num_fixed++;
    }
    if (!App_RailMonitoring__24V_ACC_VoltageCriticalCheck(rail_monitor))
    {
        // ACC
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        num_fixed++;
    }
    if (!App_RailMonitoring__22V_AUX_VoltageCriticalCheck(rail_monitor))
    {
        // AUX
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        num_fixed++;
    }

    if (num_fixed == 7)
        return true;
    return false;
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct PdmWorld *      world        = App_SharedStateMachine_GetWorld(state_machine);
    struct RailMonitoring *rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *         efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *         efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *         efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *         efuse4       = App_PdmWorld_GetEfuse4(world);
    bool                   no_faults    = Faults(efuse1, efuse2, efuse3, efuse4, rail_monitor);

    if (no_faults)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
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
