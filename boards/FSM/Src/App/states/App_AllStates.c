#include "states/App_AllStates.h"

enum HeartbeatOneHot GetHeartbeat(struct FsmCanRxInterface *can_rx_interface)
{
    enum HeartbeatOneHot heartbeat_check = 0;
    uint8_t              bms_heartbeat =
            App_CanRx_BMS_HEARTBEAT_GetSignal_HEARTBEAT(can_rx_interface);
    if (bms_heartbeat == 1)
    {
        heartbeat_check |= BMS_HEARTBEAT_ONE_HOT;
        App_CanRx_BMS_HEARTBEAT_SetSignal_HEARTBEAT(can_rx_interface, 0);
    }
    return (enum HeartbeatOneHot)heartbeat_check;
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence =
        App_FsmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanRxInterface *can_rx      = App_FsmWorld_GetCanRx(world);
    struct FsmCanTxInterface *can_tx      = App_FsmWorld_GetCanTx(world);
    struct HeartbeatMonitor *heartbeat_monitor = App_FsmWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_DUMMY_VARIABLE(can_tx, true);

    App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, GetHeartbeat(can_rx));

    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
}
