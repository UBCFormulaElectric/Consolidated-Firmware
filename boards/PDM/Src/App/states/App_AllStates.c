#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_CanTx.h"

#define NUM_CYCLES_TO_SETTLE (3U)

bool Low_Voltage_Battery_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world             = App_SharedStateMachine_GetWorld(state_machine);
    struct LowVoltageBattery *lowVoltageBattery = App_PdmWorld_GetLowVoltageBattery(world);

    bool charge_fault_status = !App_LowVoltageBattery_HasChargeFault(lowVoltageBattery);
    bool boost_fault_status  = !App_LowVoltageBattery_HasBoostControllerFault(lowVoltageBattery);

    if (charge_fault_status == 0 || boost_fault_status == 0)
        return false;
    return true;
}

void Rail_Voltages_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world        = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx       = App_PdmWorld_GetCanTx(world);
    struct RailMonitoring *   rail_monitor = App_PdmWorld_GetRailMonitoring(world);

    // Main Rail CAN_TX:
    App_CanTx_SetPeriodicSignal_RAIL_VBAT_VOLTAGE(can_tx, App_RailMonitoring_Get_VBAT_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal_RAIL_24_V_ACC_VOLTAGE(can_tx, App_RailMonitoring_Get__24V_ACC_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal_RAIL_22_V_AUX_VOLTAGE(can_tx, App_RailMonitoring_Get__22V_AUX_Voltage(rail_monitor));

}

void Efuse_Currents_CANTX(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);
    struct Efuse *            efuse1 = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2 = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3 = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4 = App_PdmWorld_GetEfuse4(world);

    App_CanTx_SetPeriodicSignal_EFUSE1_AIR_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse1));
    App_CanTx_SetPeriodicSignal_EFUSE1_LV_PWR_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse1));
    App_CanTx_SetPeriodicSignal_EFUSE2_EMETER_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse2));
    App_CanTx_SetPeriodicSignal_EFUSE2_AUX_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse2));
    App_CanTx_SetPeriodicSignal_EFUSE3_LEFT_INVERTER_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse3));
    App_CanTx_SetPeriodicSignal_EFUSE3_RIGHT_INVERTER_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse3));
    App_CanTx_SetPeriodicSignal_EFUSE4_DRS_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse4));
    App_CanTx_SetPeriodicSignal_EFUSE4_FAN_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse4));


}

bool Efuse_FaultProcedure_Channel0(struct Efuse *efuse, int max_attempts)
{
    static TimerChannel efuse_timer;
    static int          num_attempts = 0;

    if (App_Efuse_Channel0_CurrentTooHigh(efuse) == true)
    {
        App_Timer_InitTimer(efuse_timer, 1000);
        if (App_Timer_GetCurrentTimeMS() >= 1000)
        {
            App_Efuse_StandbyReset(efuse);
            if (App_Efuse_GetChannel0Current(efuse) != 0)
            {
                num_attempts = 0;
                return true;
            }
            else
            {
                num_attempts++;
                App_Timer_ResetTimer(efuse_timer);
            }
            if (num_attempts >= max_attempts)
                return false;
        }
    }

    return true;
}

bool Efuse_AIR_FaultProcedure_Check(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse1 = App_PdmWorld_GetEfuse1(world);

    if (Efuse_FaultProcedure_Channel0(efuse1, 3) == false)
    {
        return false;
    }
}

bool Efuse_EMETER_FaultProcedure_Check(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse2 = App_PdmWorld_GetEfuse2(world);

    if (App_Efuse_Channel0_CurrentTooHigh(efuse2) == true)
        return false;
    return true;

}

bool Efuse_Inverter_FaultProcedure_Check(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse3 = App_PdmWorld_GetEfuse3(world);

    if (App_Efuse_Channel0_CurrentTooHigh(efuse3) == true || App_Efuse_Channel1_CurrentTooHigh(efuse3) == true)
        return false;
    return true;
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world      = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx     = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx     = App_PdmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);


    Efuse_Currents_CANTX(state_machine);
    Rail_Voltages_CANTX(state_machine);


    App_CanTx_SetPeriodicSignal_HEARTBEAT (can_tx, true);

    /*
    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
    */

    if ()
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        return false;
    }
    return true;
}
