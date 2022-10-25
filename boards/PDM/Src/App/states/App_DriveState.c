
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_InitState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_FaultState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx_interface = App_PdmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct PdmWorld *         world      = App_SharedStateMachine_GetWorld(state_machine);
        struct PdmCanTxInterface *can_tx     = App_PdmWorld_GetCanTx(world);
        struct PdmCanRxInterface *can_rx     = App_PdmWorld_GetCanTx(world);
        struct HeartbeatMonitor * hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);
        struct LoadSwitch       * load_switch= App_PdmWorld_GetLoadSwitch(world);

        App_SetPeriodicCanSignals_Imd(can_tx, imd);

        if (!App_Airs_IsAirNegativeClosed(airs))
        {
            // if AIR- opens, go back to fault state (AIR+ will be opened there)
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
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
