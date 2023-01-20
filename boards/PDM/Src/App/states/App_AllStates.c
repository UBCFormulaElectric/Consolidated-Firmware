#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_CanTx.h"
#include "App_Timer.h"

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

void Rail_Voltages_CANTX(struct PdmCanTxInterface *can_tx, struct RailMonitoring *rail_monitor)
{

    // Main Rail CAN_TX:
    App_CanTx_SetPeriodicSignal_RAIL_VBAT_VOLTAGE(can_tx, App_RailMonitoring_Get_VBAT_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal_RAIL_24_V_ACC_VOLTAGE(can_tx, App_RailMonitoring_Get__24V_ACC_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal_RAIL_22_V_AUX_VOLTAGE(can_tx, App_RailMonitoring_Get__22V_AUX_Voltage(rail_monitor));

}

void Efuse_Currents_CANTX(struct PdmCanTxInterface *can_tx, struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{

    App_CanTx_SetPeriodicSignal_EFUSE1_AIR_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse1));
    App_CanTx_SetPeriodicSignal_EFUSE1_LV_PWR_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse1));
    App_CanTx_SetPeriodicSignal_EFUSE2_EMETER_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse2));
    App_CanTx_SetPeriodicSignal_EFUSE2_AUX_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse2));
    App_CanTx_SetPeriodicSignal_EFUSE3_LEFT_INVERTER_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse3));
    App_CanTx_SetPeriodicSignal_EFUSE3_RIGHT_INVERTER_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse3));
    App_CanTx_SetPeriodicSignal_EFUSE4_DRS_CURRENT(can_tx, App_Efuse_GetChannel0Current(efuse4));
    App_CanTx_SetPeriodicSignal_EFUSE4_FAN_CURRENT(can_tx, App_Efuse_GetChannel1Current(efuse4));

}

int Efuse_FaultProcedure_Channel0(struct Efuse *efuse, int max_attempts)
{
    static TimerChannel *efuse_timer;
    static int num_attempts = 0;
    static bool has_timer_started = false;

    if (App_Efuse_Channel0_CurrentHighCheck(efuse) == true && has_timer_started == false) {
        App_Timer_InitTimer(efuse_timer, 1000);
        has_timer_started = true;
    }

    if (has_timer_started == true)
    {
        if (App_Timer_GetCurrentTimeMS() >= 1000)
        {
            App_Efuse_StandbyReset(efuse);
            if (App_Efuse_GetChannel0Current(efuse) != 0) {
                App_Timer_Stop(efuse_timer);
                has_timer_started = false;
                num_attempts = 0;
                return 0; // everything is fine
            }
            else {
                num_attempts++;

                if (num_attempts >= max_attempts) {
                    App_Timer_Stop(efuse_timer);
                    has_timer_started = false;
                    return 2; //channel failed
                }
                else
                    App_Timer_Restart(efuse_timer);
            }

        }
    }

    return 1; //unknown status

}

int Efuse_FaultProcedure_Channel1(struct Efuse *efuse, int max_attempts)
{
    static TimerChannel *efuse_timer;
    static int num_attempts = 0;
    static bool has_timer_started = false;

    if (App_Efuse_Channel1_CurrentHighCheck(efuse) == true && has_timer_started == false) {
        App_Timer_InitTimer(efuse_timer, 1000);
        has_timer_started = true;
    }

    if (has_timer_started == true)
    {
        if (App_Timer_GetCurrentTimeMS() >= 1000)
        {
            App_Efuse_StandbyReset(efuse);
            if (App_Efuse_GetChannel1Current(efuse) != 0) {
                App_Timer_Stop(efuse_timer);
                has_timer_started = false;
                num_attempts = 0;
                return 0; // everything is fine
            }
            else {
                num_attempts++;

                if (num_attempts >= max_attempts) {
                    App_Timer_Stop(efuse_timer);
                    has_timer_started = false;
                    return 2; //channel failed
                }
                else
                    App_Timer_Restart(efuse_timer);
            }

        }
    }

    return 1; //unknown status

}

bool Efuse_AIR_FaultProcedure_Check(struct Efuse * efuse)
{

    if (Efuse_FaultProcedure_Channel0(efuse, 3) == 2)
    {
        return false;
    }
    return true;
}

bool Efuse_LVPWR_FaultProcedure_Check(struct Efuse * efuse)
{

    if (Efuse_FaultProcedure_Channel1(efuse, 3) == 2)
    {
        return false;
    }
    return true;
}

bool Efuse_EMETER_FaultProcedure_Check(struct Efuse * efuse)
{

    if (App_Efuse_Channel0_CurrentHighCheck(efuse) == true)
        return false;
    return true;
}

bool Efuse_DRS_FaultProcedure_Check(struct Efuse * efuse)
{

    if (Efuse_FaultProcedure_Channel0(efuse, 3) == 2)
    {
        return false;
    }
    return true;
}

bool Efuse_FAN_FaultProcedure_Check(struct Efuse * efuse)
{

    if (Efuse_FaultProcedure_Channel1(efuse, 3) == 2)
    {
        return false;
    }
    return true;
}

bool Efuse_Inverter_FaultProcedure_Check(struct Efuse * efuse)
{

    if (App_Efuse_Channel0_CurrentHighCheck(efuse) == true || App_Efuse_Channel1_CurrentHighCheck(efuse) == true)
        return false;
    return true;
}

void Efuse_FaultProcedure_Checks(struct Efuse *efuse1, struct Efuse *efuse2, struct Efuse *efuse3, struct Efuse *efuse4)
{
    Efuse_AIR_FaultProcedure_Check(efuse1);
    Efuse_LVPWR_FaultProcedure_Check(efuse1);
    Efuse_EMETER_FaultProcedure_Check(efuse2);
    Efuse_Inverter_FaultProcedure_Check(efuse3);
    Efuse_DRS_FaultProcedure_Check(efuse4);
    Efuse_FAN_FaultProcedure_Check(efuse4);

    if (!Efuse_AIR_FaultProcedure_Check(efuse1) || !Efuse_EMETER_FaultProcedure_Check(efuse2) || !Efuse_Inverter_FaultProcedure_Check(efuse3))
    {
        // HARD SHUTDOWN

    }
}

void HeartbeatCheck(struct PdmCanTxInterface *can_tx, struct PdmCanRxInterface *can_rx, struct HeartbeatMonitor *hb_monitor)
{
    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        // JSONCAN -> App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);
    }

    else
    {
        // TODO: JSONCAN -> (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        //        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world         = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx     = App_PdmWorld_GetCanTx(world);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    struct RailMonitoring *rail_monitor  = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *            efuse1     = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2     = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3     = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4     = App_PdmWorld_GetEfuse4(world);


    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    Efuse_Currents_CANTX(can_tx, efuse1, efuse2, efuse3, efuse4);
    Rail_Voltages_CANTX(can_tx, rail_monitor);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *        world      = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor *hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);
    struct PdmCanTxInterface *can_tx    = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx    = App_PdmWorld_GetCanRx(world);
    struct Efuse *            efuse1    = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2    = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3    = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4    = App_PdmWorld_GetEfuse4(world);



    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    // Fault Procedures
    Efuse_FaultProcedure_Checks(efuse1, efuse2, efuse3, efuse4);

    HeartbeatCheck(can_tx, can_rx, hb_monitor);





    if ()
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        return false;
    }
    return true;
}
