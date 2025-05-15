#include "app_stateMachine.h"
#include "app_charger.h"
#include "io_charger.h"

static void app_initStateRunOnEntry()
{
    
}

static void app_initStateRunOnTick1Hz()
{
    app_allStates_runOnTick1Hz();
}

static void app_initStateRunOnTick100Hz()
{
    if(app_allStates_runOnTick100Hz)
    {
       const ConnectionStatus EVSE_connection_status = io_charger_getConnectionStatus();
       const bool Elcon_connection_status = 
       const bool clear_Elcon_latch = 

       const bool precharge_for_charging = EVSE_connection_status && Elcon_connection_status;
       if(precharge_for_charging)
       {
            app_stateMachine_setNextState(app_prechargeChargingState_get())
       }
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name               = "INIT",
        .run_on_entry       = app_initStateRunOnEntry,
        .run_on_tick_1Hz    = app_initStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_initStateRunOnTick100Hz,
        .run_on_exit        = NULL,
    };

    return &init_state;
}