#include "app_precharge.h"
#include "app_stateMachine.h"
#include "app_tractiveSystem.h"
#include "app_initState.h"
#include "io_charger.h"
#include "app_segments.h"
#include "io_irs.h"
#include "io_time.h"
#include "app_timer.h"
#include "app_canRx.h"
#include "states/app_allStates.h"
#include "states/app_chargeState.h"
#include <app_canTx.h>
#include "app_segments.h"
#include "states/app_faultState.h"

#define PRECHARGE_ACC_VOLTAGE_THRESHOLD 0.9f
#define NUM_OF_INVERTERS 4U
#define INVERTER_CAPACITANCE_F 75e-6f
#define NUM_OF_RESISTORS 1U
#define PRECHARGE_RESISTANCE_OHMS 3e3f
#define S_TO_MS 1000U
#define PRECHARGE_RC_MS \
    (S_TO_MS * (PRECHARGE_RESISTANCE_OHMS * NUM_OF_RESISTORS * INVERTER_CAPACITANCE_F * NUM_OF_INVERTERS))
#define MAX_PRECHARGE_ATTEMPTS 3U
#define PRECHARGE_COMPLETION_MS (float)PRECHARGE_RC_MS * 2.7f // 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_UPPERBOUND_MS (uint32_t)(PRECHARGE_COMPLETION_MS * 3.0f)
#define PRECHARGE_COMPLETION_LOWERBOUND_MS (uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f)

static void app_prechargeChargeStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_CHARGE_STATE);

    io_irs_closePrecharge();
    app_precharge_restart();
}

static void app_prechargeChargeStateRunOnTick100Hz(void)
{
    const PrechargeState state = app_precharge_poll(true);

    if (state == PRECHARGE_STATE_FAILED_CRITICAL)
    {
        // Just in case we exited charging not due to CAN (fault, etc.) set the CAN table back to false so we don't
        // unintentionally re-enter charge state.
        app_canRx_Debug_StartCharging_update(false);

        app_stateMachine_setNextState(app_faultState_get());
    }
    else if (state == PRECHARGE_STATE_FAILED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (state == PRECHARGE_STATE_SUCCESS)
    {
        // Precharge successful, close positive contactor.
        io_irs_closePositive();

        app_stateMachine_setNextState(app_chargeState_get());
    }

    // TODO: Detect charger via PWM.
    // const bool is_charger_connected = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);
    // if (!is_charger_connected)
    // {
    //     app_stateMachine_setNextState(app_initState_get());
    // }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void app_prechargeChargeStateRunOnExit(void)
{
    io_irs_openPrecharge();
}

const State *app_prechargeChargeState_get(void)
{
    static State precharge_charge_state = {
        .name              = "PRECHARGE CHARGE",
        .run_on_entry      = app_prechargeChargeStateRunOnEntry,
        .run_on_tick_100Hz = app_prechargeChargeStateRunOnTick100Hz,
        .run_on_exit       = app_prechargeChargeStateRunOnExit,
    };

    return &precharge_charge_state;
}
