#include "app_stateMachine.h"
#include "io_charger.h"

static void app_chargeInitStateRunOnEntry()
{

}

static void app_chargeInitStateRunOnTick1Hz()
{
    ConnectionStatus EVSE_connection_status = io_charger_getConnectionStatus();

    bool Elcon_hardware_failure = app_canRx_Elcon_HardwareFailure_get();
    bool Elcon_over_temp = app_canRx_Elcon_ChargerOverTemperature_get();
    bool Elcon_input_voltage_error = app_canRx_Elcon_InputVoltageError_get();
    bool Elcon_charging_state = app_canRx_Elcon_ChargingState_get();
    bool Elcon_comm_timeout = app_canRx_Elcon_CommunicationTimeout_get();

    if(EVSE_connection_status == EVSE_CONNECTED 
        && !Elcon_hardware_failure 
        && !Elcon_over_temp 
        && !Elcon_input_voltage_error 
        && !Elcon_charging_state 
        && !Elcon_comm_timeout)
    {
        app_stateMachine_setNextState(charge_state);
    }
}

static void app_chargeInitStateRunOnTick100Hz()
{
    
}

static void app_chargeInitStateRunOnExit()
{

}

const State *app_chargeState_get(void)
{
    static State charge_init_state = {
        .name               = "CHARGE INIT",
        .run_on_entry       = app_chargeInitStateRunOnEntry,
        .run_on_tick_1Hz    = app_chargeInitStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_chargeInitStateRunOnTick100Hz,
        .run_on_exit        = app_chargeInitStateRunOnExit,
    };

    return &charge_init_state;
}