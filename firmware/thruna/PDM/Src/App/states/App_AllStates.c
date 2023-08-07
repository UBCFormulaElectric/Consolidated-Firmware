#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world      = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor * hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);
    struct LowVoltageBattery *lv_battery = App_PdmWorld_GetLowVoltageBattery(world);

    App_CanTx_PDM_Vitals_Heartbeat_Set(true);
    App_CanTx_PDM_Voltages_BatVoltage_Set(App_LowVoltageBattery_GetBatVoltage(lv_battery));
    App_CanTx_PDM_Voltages_AccVoltage_Set(App_LowVoltageBattery_GetAccVoltage(lv_battery));
    App_CanTx_PDM_Voltages_BoostVoltage_Set(App_LowVoltageBattery_GetBoostVoltage(lv_battery));

    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }
}
