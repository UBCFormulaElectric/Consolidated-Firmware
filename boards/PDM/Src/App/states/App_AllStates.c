#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_CanTx.h"
#include "App_Timer.h"
#include "App_PdmWorld.h"

#define NUM_CYCLES_TO_SETTLE (3U)
/*
void Low_Voltage_Battery_CANTX(struct LowVoltageBattery *LVB)
{
    App_CanTx_PDM_LowVoltageBattery_Faults_Charge_Fault_Set(App_LowVoltageBattery_HasChargeFault(LVB));
    App_CanTx_PDM_LowVoltageBattery_Faults_Boost_Fault_Set(App_LowVoltageBattery_HasBoostControllerFault(LVB));
}
*/
void Rail_Voltages_CANTX(struct RailMonitoring *rail_monitor)
{
    // Main Rail CAN_TX:

    App_CanTx_PDM_VBAT_Voltage_Set(App_RailMonitoring_Get_VBAT_Voltage(rail_monitor));
    App_CanTx_PDM_VBAT_Critical_Set(App_RailMonitoring_VBAT_VoltageCriticalCheck(rail_monitor));
    App_CanTx_PDM_VBAT_LOW_Set(App_RailMonitoring_VBAT_VoltageLowCheck(rail_monitor));
    App_CanTx_PDM_VBAT_HIGH_Set(App_RailMonitoring_VBAT_VoltageHighCheck(rail_monitor));

    App_CanTx_PDM__24V_ACC_Voltage_Set(App_RailMonitoring_Get__24V_ACC_Voltage(rail_monitor));
    App_CanTx_PDM__24V_ACC_Critical_Set(App_RailMonitoring__24V_ACC_VoltageCriticalCheck(rail_monitor));
    App_CanTx_PDM__24V_ACC_LOW_Set(App_RailMonitoring__24V_ACC_VoltageLowCheck(rail_monitor));
    App_CanTx_PDM__24V_ACC_HIGH_Set(App_RailMonitoring__24V_ACC_VoltageHighCheck(rail_monitor));

    App_CanTx_PDM__22V_AUX_Voltage_Set(App_RailMonitoring_Get__22V_AUX_Voltage(rail_monitor));
    App_CanTx_PDM__22V_AUX_Critical_Set(App_RailMonitoring__22V_AUX_VoltageCriticalCheck(rail_monitor));
    App_CanTx_PDM__22V_AUX_LOW_Set(App_RailMonitoring__22V_AUX_VoltageLowCheck(rail_monitor));
    App_CanTx_PDM__22V_AUX_HIGH_Set(App_RailMonitoring__22V_AUX_VoltageHighCheck(rail_monitor));
}

void Efuse_Currents_CANTX(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{
    App_CanTx_PDM_Efuse1_Currents_AIR_Current_Set(App_Efuse_GetChannel0Current(efuse1));
    App_CanTx_PDM_Efuse1_Currents_LVPWR_Current_Set(App_Efuse_GetChannel1Current(efuse1));
    App_CanTx_PDM_Efuse2_Currents_EMETER_Current_Set(App_Efuse_GetChannel0Current(efuse2));
    App_CanTx_PDM_Efuse2_Currents_AUX_Current_Set(App_Efuse_GetChannel1Current(efuse2));
    App_CanTx_PDM_Efuse3_Currents_LEFT_INVERTER_Current_Set(App_Efuse_GetChannel0Current(efuse3));
    App_CanTx_PDM_Efuse3_Currents_RIGHT_INVERTER_Current_Set(App_Efuse_GetChannel1Current(efuse3));
    App_CanTx_PDM_Efuse4_Currents_DRS_Current_Set(App_Efuse_GetChannel0Current(efuse4));
    App_CanTx_PDM_Efuse4_Currents_FAN_Current_Set(App_Efuse_GetChannel1Current(efuse4));

}

void HeartbeatCheck(struct HeartbeatMonitor *hb_monitor)
{
    //    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    if (false)
    {
        // JSONCAN -> App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);
    }

    else
    {
        // TODO: JSONCAN -> (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
        // App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        //        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world        = App_SharedStateMachine_GetWorld(state_machine);
    struct RailMonitoring *rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *         efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *         efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *         efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *         efuse4       = App_PdmWorld_GetEfuse4(world);

    Efuse_Currents_CANTX(efuse1, efuse2, efuse3, efuse4);
    Rail_Voltages_CANTX(rail_monitor);

    // App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);
    // HeartbeatCheck(can_tx, can_rx, hb_monitor);
}
