#include "states/App_AllStates.h"
#include "App_CanTx.h"
#include "App_Timer.h"
#include "App_PdmWorld.h"

#define NUM_CYCLES_TO_SETTLE (3U)

void Rail_Voltages_CANTX(struct RailMonitoring *rail_monitor)
{
    // Main Rail CAN_TX:

    App_CanTx_PDM_VBAT_Voltage_Set(App_RailMonitoring_GetVbatVoltage(rail_monitor));
    App_CanTx_PDM_VBAT_LowCheck_Set(App_RailMonitoring_VbatVoltageLowCheck(rail_monitor));
    App_CanTx_PDM_VBAT_HighCheck_Set(App_RailMonitoring_VbatVoltageHighCheck(rail_monitor));

    App_CanTx_PDM_24V_ACC_Voltage_Set(App_RailMonitoring_Get24VAccumulatorVoltage(rail_monitor));
    App_CanTx_PDM_24V_ACC_LowCheck_Set(App_RailMonitoring_24VAccumulatorVoltageLowCheck(rail_monitor));
    App_CanTx_PDM_24V_ACC_HighCheck_Set(App_RailMonitoring_24VAccumulatorVoltageHighCheck(rail_monitor));

    App_CanTx_PDM_22V_AUX_Voltage_Set(App_RailMonitoring_Get22VAuxiliaryVoltage(rail_monitor));
    App_CanTx_PDM_22V_AUX_LowCheck_Set(App_RailMonitoring_22VAuxiliaryVoltageLowCheck(rail_monitor));
    App_CanTx_PDM_22V_AUX_HighCheck_Set(App_RailMonitoring_22VAuxiliaryVoltageHighCheck(rail_monitor));
}

void Efuse_Currents_CANTX(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{
    App_CanTx_PDM_Efuse1_Current_AIR_Set(App_Efuse_GetChannel0Current(efuse1));
    App_CanTx_PDM_Efuse1_Current_LVPWR_Set(App_Efuse_GetChannel1Current(efuse1));
    App_CanTx_PDM_Efuse2_Current_EMETER_Set(App_Efuse_GetChannel0Current(efuse2));
    App_CanTx_PDM_Efuse2_Current_AUX_Set(App_Efuse_GetChannel1Current(efuse2));
    App_CanTx_PDM_Efuse3_Current_LEFT_INVERTER_Set(App_Efuse_GetChannel0Current(efuse3));
    App_CanTx_PDM_Efuse3_Current_RIGHT_INVERTER_Set(App_Efuse_GetChannel1Current(efuse3));
    App_CanTx_PDM_Efuse4_Current_DRS_Set(App_Efuse_GetChannel0Current(efuse4));
    App_CanTx_PDM_Efuse4_Current_FAN_Set(App_Efuse_GetChannel1Current(efuse4));
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
}
