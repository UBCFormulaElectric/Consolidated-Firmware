#include "app_stateMachine.h"
#include "app_chargeState.h"
#include "io_charger.h"
#include "stdlib.h"

static void app_chargeInitStateRunOnEntry()
{

}

static void app_chargeInitStateRunOnTick1Hz()
{
    
}

static void app_chargeInitStateRunOnTick100Hz()
{
    const bool extShutdown   = !io_irs_isNegativeClosed();
    const bool chargerConn   = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);
    const bool userEnable    = app_canRx_Debug_StartCharging_get();

    ElconRx rx = readElconStatus();

    const bool fault = extShutdown                  ||
                       !chargerConn                 ||
                       rx.hardwareFailure           ||
                       rx.overTemperature           ||
                       rx.inputVoltageFault         ||
                       rx.chargingStateFault        ||
                       rx.commTimeout;

    if (fault)
        app_stateMachine_setNextState(app_chargeFaultState_get());
    else if (!userEnable)
        app_stateMachine_setNextState(app_chargeInitState_get());
    else
        app_stateMachine_setNextState(app_chargeState_get());
}

static void app_chargeInitStateRunOnExit()
{

}

const State *app_chargeInitState_get(void)
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