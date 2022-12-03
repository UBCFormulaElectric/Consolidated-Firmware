#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_CanTx.h"

#define NUM_CYCLES_TO_SETTLE (3U)

bool Low_Voltage_Battery_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *        world              = App_SharedStateMachine_GetWorld(state_machine);
    struct LowVoltageBattery* lowVoltageBattery = App_PdmWorld_GetLowVoltageBattery(world);

    bool charge_fault_status                    = !App_LowVoltageBattery_HasChargeFault(lowVoltageBattery);
    bool boost_fault_status                     = !App_LowVoltageBattery_HasBoostControllerFault(lowVoltageBattery);

    if (charge_fault_status == 0 || boost_fault_status == 0)
        return false;
    return true;
}

bool Rail_Voltages_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world        = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx       = App_PdmWorld_GetCanTx(world);
    struct RailMonitoring *   rail_monitor = App_PdmWorld_GetRailMonitoring(world);

    bool VBAT_status                       = App_RailMonitoring_VBAT_VoltageCheck(rail_monitor);
    bool __24_V_ACC_status                 = App_RailMonitoring__24V_ACC_VoltageCheck(rail_monitor);
    bool __22_V_AUX_status                 = App_RailMonitoring__22V_AUX_VoltageCheck(rail_monitor);

    // Main Rail CAN_TX:
    App_CanTx_SetPeriodicSignal_RAIL_VBAT_VOLTAGE_OUT_OF_RANGE(can_tx, VBAT_status);
    App_CanTx_SetPeriodicSignal_RAIL_24_V_ACC_VOLTAGE_OUT_OF_RANGE(can_tx, __24_V_ACC_status);
    App_CanTx_SetPeriodicSignal_RAIL_22_V_AUX_VOLTAGE_OUT_OF_RANGE(can_tx, __22_V_AUX_status);

    if (VBAT_status == 0 || __24_V_ACC_status == 0 || __22_V_AUX_status == 0)
        return false;
    return true;
}

bool Efuse_Currents_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world         = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx        = App_PdmWorld_GetCanTx(world);
    struct Efuse *            efuse1        = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2        = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3        = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4        = App_PdmWorld_GetEfuse4(world);
    
    bool efuse1_ch0_status = App_Efuse_Channel0_CurrentCheck(efuse1);
    bool efuse1_ch1_status = App_Efuse_Channel1_CurrentCheck(efuse1);
    bool efuse2_ch0_status = App_Efuse_Channel0_CurrentCheck(efuse2);
    bool efuse2_ch1_status = App_Efuse_Channel1_CurrentCheck(efuse2);
    bool efuse3_ch0_status = App_Efuse_Channel0_CurrentCheck(efuse3);
    bool efuse3_ch1_status = App_Efuse_Channel1_CurrentCheck(efuse3);
    bool efuse4_ch0_status = App_Efuse_Channel0_CurrentCheck(efuse4);
    bool efuse4_ch1_status = App_Efuse_Channel1_CurrentCheck(efuse4);
    

    // Load Switch Current CAN_TX:
    App_CanTx_SetPeriodicSignal_EFUSE1_AIR_CURRENT_OUT_OF_RANGE(can_tx, efuse1_ch0_status);
    App_CanTx_SetPeriodicSignal_EFUSE1_LV_PWR_CURRENT_OUT_OF_RANGE(can_tx, efuse1_ch1_status);
    App_CanTx_SetPeriodicSignal_EFUSE2_EMETER_CURRENT_OUT_OF_RANGE(can_tx, efuse2_ch0_status);
    App_CanTx_SetPeriodicSignal_EFUSE2_AUX_CURRENT_OUT_OF_RANGE(can_tx, efuse2_ch1_status);
    App_CanTx_SetPeriodicSignal_EFUSE3_LEFT_INVERTER_CURRENT_OUT_OF_RANGE(can_tx, efuse3_ch0_status);
    App_CanTx_SetPeriodicSignal_EFUSE3_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE(can_tx, efuse3_ch1_status);
    App_CanTx_SetPeriodicSignal_EFUSE4_DRS_CURRENT_OUT_OF_RANGE(can_tx, efuse4_ch0_status);
    App_CanTx_SetPeriodicSignal_EFUSE4_FAN_CURRENT_OUT_OF_RANGE(can_tx, efuse4_ch1_status);
    
    if (efuse1_ch0_status == 0 || efuse1_ch1_status == 0 ||
        efuse2_ch0_status == 0 || efuse2_ch1_status == 0 ||
        efuse3_ch0_status == 0 || efuse3_ch1_status == 0 ||
        efuse4_ch0_status == 0 || efuse4_ch1_status == 0)
        return false;
    return true;
}

bool Efuse_FaultProcedure_Channel0(struct Efuse *efuse, int max_attempts)
{
    if (App_Efuse_Channel0_CurrentCheck(efuse) == false)
        //start fault timer
    if (App_Efuse)
}
bool EfuseINV(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world               = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world               = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence    = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world           = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx          = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx          = App_PdmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor      = App_PdmWorld_GetHeartbeatMonitor(world);


    static uint8_t acc_meas_settle_count      = 0U;
    bool battery_status                       = Low_Voltage_Battery_CANTX(state_machine);
    bool rail_status                          = Rail_Voltages_CANTX(state_machine);
    bool efuse_status                         = Efuse_Currents_CANTX(state_machine);

    // HB CANTX
    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    /*
    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
    */
    if (acc_meas_settle_count < NUM_CYCLES_TO_SETTLE)
    {
        acc_meas_settle_count++;
    }
    else if (battery_status == false || rail_status == false || efuse_status == false)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        return false;
    }
    return true;
}
