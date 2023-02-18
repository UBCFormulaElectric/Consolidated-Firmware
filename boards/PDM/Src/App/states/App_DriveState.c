
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"
#include "states/App_FaultState.h"

#define NUM_CYCLES_TO_SETTLE 0U
#define EFUSE_CURRENT_THRESHOLD (0.1f)

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

void Efuse_Enable_Channels_18650_STARTUP(
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

void Efuse_Enable_Channels_PCM_RUNNING(
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
    App_Efuse_EnableChannel0(efuse4);
    App_Efuse_EnableChannel1(efuse4);
}

void Warning_Detection(
    struct Efuse *         efuse1,
    struct Efuse *         efuse2,
    struct Efuse *         efuse3,
    struct Efuse *         efuse4,
    struct RailMonitoring *rail_monitor)
{
    if (App_Efuse_FaultProcedure_Channel1(efuse2, 3) == 2)
    {
        // LVPWR
        App_CanTx_PDM_Efuse_Fault_Checks_LVPWR_Set(true);
    }
    else if (App_Efuse_FaultProcedure_Channel1(efuse2, 3) == 0)
    {
        App_CanTx_PDM_Efuse_Fault_Checks_LVPWR_Set(false);
    }

    if (App_Efuse_FaultProcedure_Channel0(efuse4, 3) == 2)
    {
        // DRS
        App_CanTx_PDM_Efuse_Fault_Checks_DRS_Set(true);
    }
    else if (App_Efuse_FaultProcedure_Channel0(efuse4, 3) == 0)
    {
        App_CanTx_PDM_Efuse_Fault_Checks_DRS_Set(false);
    }

    if (App_Efuse_FaultProcedure_Channel1(efuse4, 3) == 2)
    {
        // FAN
        App_CanTx_PDM_Efuse_Fault_Checks_FAN_Set(true);
    }
    else if (App_Efuse_FaultProcedure_Channel1(efuse4, 3) == 0)
    {
        App_CanTx_PDM_Efuse_Fault_Checks_FAN_Set(true);
    }
}

bool Fault_Detection(
    struct Efuse *         efuse1,
    struct Efuse *         efuse2,
    struct Efuse *         efuse3,
    struct Efuse *         efuse4,
    struct RailMonitoring *rail_monitor)
{
    bool status = false;
    if (App_Efuse_FaultProcedure_Channel0(efuse1, 3) == 2)
    {
        // AIR
        App_CanTx_PDM_Efuse_Fault_Checks_AIR_Set(true);
        status = true;
    }
    if (App_Efuse_FaultProcedure_Channel0(efuse2, 0) == 2)
    {
        // EMETER
        App_CanTx_PDM_Efuse_Fault_Checks_EMETER_Set(true);
        status = true;
    }
    if (App_Efuse_FaultProcedure_Channel0(efuse3, 0) == 2)
    {
        // LEFT INVERTER
        App_CanTx_PDM_Efuse_Fault_Checks_LEFT_INVERTER_Set(true);
        status = true;
    }
    if (App_Efuse_FaultProcedure_Channel1(efuse3, 0) == 2)
    {
        // LEFT INVERTER
        App_CanTx_PDM_Efuse_Fault_Checks_RIGHT_INVERTER_Set(true);
        status = true;
    }
    if (App_RailMonitoring_VBAT_VoltageCriticalCheck(rail_monitor))
    {
        // VBAT
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        status = true;
    }
    if (App_RailMonitoring__24V_ACC_VoltageCriticalCheck(rail_monitor))
    {
        // ACC
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        status = true;
    }
    if (App_RailMonitoring__22V_AUX_VoltageCriticalCheck(rail_monitor))
    {
        // AUX
        // NO CAN MESSAGE BECAUSE IT'S IN ALLSTATES
        status = true;
    }

    return status;
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world        = App_SharedStateMachine_GetWorld(state_machine);
    struct RailMonitoring *rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *         efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *         efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *         efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *         efuse4       = App_PdmWorld_GetEfuse4(world);
    bool                   has_fault    = Fault_Detection(efuse1, efuse2, efuse3, efuse4, rail_monitor);

    App_AllStatesRunOnTick100Hz(state_machine);
    if (!has_fault)
    {
        Warning_Detection(efuse1, efuse2, efuse3, efuse4, rail_monitor);

        if (1 == 1) // To be replaced with App_CanTx_BMSStates_AIRNEGATIVE closed
        {
            // PCM Running
            Efuse_Enable_Channels_PCM_RUNNING(efuse1, efuse2, efuse3, efuse4);
        }
        else
        {
            // 18650 Startup
            Efuse_Enable_Channels_18650_STARTUP(efuse1, efuse2, efuse3, efuse4);
        }
    }
    else
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
