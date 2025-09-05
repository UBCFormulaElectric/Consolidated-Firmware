#include "app_stateMachine.h"
#include "app_prechargeChargeState.h"
#include "app_tractiveSystem.h"
#include "io_faultLatch.h"
#include "io_irs.h"
#include "states/app_allStates.h"
#include "states/app_balancingState.h"
#include "states/app_prechargeDriveState.h"
#include "stdio.h"
#include "app_canRx.h"
#include <app_canTx.h>
#include <app_canUtils.h>

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void app_initStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INIT_STATE);
    io_faultLatch_setCurrentStatus(&bms_ok_latch, true);

    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_irs_openPositive();
}

static void app_initStateRunOnTick100Hz(void)
{
    const bool is_irs_negative_closed = io_irs_isNegativeClosed();
    const bool ts_discharged          = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;
    const bool cell_balancing_enabled = app_canRx_Debug_CellBalancingRequest_get();
    const bool vc_ready_for_precharge = app_canRx_VC_State_get() == VC_BMS_ON_STATE;

    // TODO: Detect charger via PWM.
    // const bool is_charger_connected      = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);

    if (ts_discharged)
    {
        const bool external_charging_request = app_canRx_Debug_StartCharging_get();

        if (external_charging_request)
        {
            app_stateMachine_setNextState(app_prechargeChargeState_get());
        }
        else if (vc_ready_for_precharge)
        {
            app_stateMachine_setNextState(app_prechargeDriveState_get());
        }
        else if (cell_balancing_enabled)
        {
            app_stateMachine_setNextState(app_balancingState_get());
        }
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = app_initStateRunOnEntry,
        .run_on_tick_100Hz = app_initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
