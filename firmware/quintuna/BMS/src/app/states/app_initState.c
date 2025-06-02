#include "io_irs.h"
#include "io_faultLatch.h"
#include "app_heartbeatMonitors.h"
#include "app_tractiveSystem.h"
#include "app_canRx.h"
#include "states/app_balancingState.h"
#include "states/app_prechargeForDriveState.h"
#include "states/app_prechargeForChargeState.h"
#include "app_canTx.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INIT_STATE);
    io_faultLatch_setCurrentStatus(&bms_ok_latch, true);

    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_irs_openPositive();
}

static void runOnTick100Hz(void)
{
    const bool air_negative_closed = io_irs_isNegativeClosed();
    const bool ts_discharged       = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;
    const bool missing_hb          = app_heartbeatMonitor_isSendingMissingHeartbeatFault(&hb_monitor);

    if (air_negative_closed && ts_discharged)
    {
        // TODO: Setup charger!
        const bool charger_connected         = true; // app_canRx_BRUSA_IsConnected_get();
        const bool cell_balancing_enabled    = app_canRx_Debug_CellBalancingRequest_get();
        const bool external_charging_request = app_canRx_Debug_StartCharging_get();

        // const bool clear_brusa_latch         = app_canRx_Debug_ClearChargerLatchedFault_get();
        // app_canTx_BMS_ClearLatch_set(clear_brusa_latch);

        const bool precharge_for_charging = charger_connected && external_charging_request;

        // TODO: This should also read from the VC if this is allowed!
        const bool precharge_for_driving = false; //! charger_connected && !cell_balancing_enabled && !missing_hb;

        if (precharge_for_charging)
        {
            app_stateMachine_setNextState(app_prechargeForChargeState_get());
        }
        else if (precharge_for_driving)
        {
            app_stateMachine_setNextState(app_prechargeForDriveState_get());
        }
        else if (cell_balancing_enabled)
        {
            app_stateMachine_setNextState(app_balancingState_get());
        }
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnEntry,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
