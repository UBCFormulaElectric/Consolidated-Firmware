#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

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

    App_CanTx_PDM_Vitals_Heartbeat_Set(true);

    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }
}

void App_AllStatesRunOnTick1kHz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}
