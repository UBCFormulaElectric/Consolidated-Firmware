#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx           = App_FsmWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence = App_FsmWorld_GetRgbLedSequence(world);
    struct Brake *            brake            = App_FsmWorld_GetBrake(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_OC(can_tx, App_Brake_IsPressureSensorOpenCircuit(brake));
    App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_SC(can_tx, App_Brake_IsPressureSensorShortCircuited(brake));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world      = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx     = App_FsmWorld_GetCanTx(world);
    struct FsmCanRxInterface *can_rx     = App_FsmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor = App_FsmWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    App_CanTx_SetPeriodicSignal_MISSING_HEARTBEAT(can_tx, !App_SharedHeartbeatMonitor_Tick(hb_monitor));
}
