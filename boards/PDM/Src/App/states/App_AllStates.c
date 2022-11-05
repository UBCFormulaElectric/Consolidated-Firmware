#include "states/App_AllStates.h"
#include "states/App_FaultState.h"


void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world             = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx            = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx            = App_PdmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor        = App_PdmWorld_GetHeartbeatMonitor(world);
    struct RailMonitoring * rail_monitor        = App_PdmWorld_GetRailMonitoring(world);
    struct LoadSwitch      * load_switch        = App_PdmWorld_GetLoadSwitch(world);

    bool status = true;

    // Main Rail Monitoring:
    App_CanTx_SetPeriodicSignal_VBAT(can_tx, App_RailMonitoring_Get_VBAT_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal__24_V_ACC(can_tx, App_RailMonitoring_Get__24V_ACC_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal__24_V_AUX(can_tx, App_RailMonitoring_Get__24V_AUX_Voltage(rail_monitor));

    // Load Switch Current Monitoring:
    App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT(can_tx, App_LoadSwitch_Get_AUX1_Current(load_switch));
    App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT(can_tx, App_LoadSwitch_Get_AUX2_Current(load_switch));
    App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT(can_tx, App_LoadSwitch_Get_LEFT_INVERTER_Current(load_switch));
    App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT(can_tx, App_LoadSwitch_Get_RIGHT_INVERTER_Current(load_switch));
    App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT(can_tx, App_LoadSwitch_Get_ENERGY_METER_Current(load_switch));
    App_CanTx_SetPeriodicSignal_CAN_CURRENT(can_tx, App_LoadSwitch_Get_CAN_Current(load_switch));
    App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT(can_tx, App_LoadSwitch_Get_AIR_SHUTDOWN_Current(load_switch));

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    if ()
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
