
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"

#define NUM_CYCLES_TO_SETTLE 0U

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx_interface = App_PdmWorld_GetCanTx(world);
}

void Efuse_Enable_Disable_Channels(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
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

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);

    struct RailMonitoring *rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *            efuse1    = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2    = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3    = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4    = App_PdmWorld_GetEfuse4(world);





    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        Efuse_Enable_Disable_Channels(efuse1, efuse2, efuse3, efuse4);

        if (App_RailMonitoring__24V_ACC_VoltageCriticalCheck(rail_monitor))
        {
            // HW SHUTDOWN
        }
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
