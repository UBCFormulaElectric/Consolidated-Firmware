#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *        world      = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor *hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);

    // JSONCAN -> App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (false) // TODO: JSONCAN -> (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        //        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
}
