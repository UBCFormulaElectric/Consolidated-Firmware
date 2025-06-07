#include "app_stateMachine.h"
#include "app_chargeState.h"
#include "app_chargeInitState.h"
#include "io_irs.h"
#include "io_charger.h"
#include "app_canRx.h"

static void app_chargeFaultStateRunOnEntry() {}

static void app_chargeFaultStateRunOnTick1Hz() {}

static void app_chargeFaultStateRunOnTick100Hz()
{
    ElconRx s = readElconStatus();

    const bool extShutdown = !io_irs_isNegativeClosed();
    const bool chargerConn = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);
    const bool userEnable  = app_canRx_Debug_StartCharging_get();

    ElconRx rx = readElconStatus();

    const bool fault = extShutdown || !chargerConn || rx.hardwareFailure || rx.overTemperature ||
                       rx.inputVoltageFault || rx.chargingStateFault || rx.commTimeout;

    if (!userEnable)
        app_stateMachine_setNextState(app_chargeInitState_get());
    else if (!fault)
        app_stateMachine_setNextState(app_chargeState_get());
}

static void app_chargeFaultStateRunOnExit() {}

const State *app_chargeFaultState_get(void)
{
    static State charge_fault_state = {
        .name              = "CHARGE FAULT",
        .run_on_entry      = app_chargeFaultStateRunOnEntry,
        .run_on_tick_1Hz   = app_chargeFaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = app_chargeFaultStateRunOnTick100Hz,
        .run_on_exit       = app_chargeFaultStateRunOnExit,
    };

    return &charge_fault_state;
}