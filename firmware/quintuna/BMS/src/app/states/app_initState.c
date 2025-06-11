#include "states/app_states.h"

#include "app_canRx.h"
#include "app_canTx.h"
#include "app_tractiveSystem.h"
// #include "app_soc.h"
// #include "app_inverterOnState.h"

#include "io_faultLatch.h"
#include "io_irs.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void initStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INIT_STATE);
    io_faultLatch_setCurrentStatus(&bms_ok_latch, true);

    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_irs_setPositive(false);
}

static void initStateRunOnTick100Hz(void)
{
    const bool irs_negative_closed    = io_irs_negativeState();
    const bool ts_discharged          = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;
    const bool cell_balancing_enabled = app_canRx_Debug_CellBalancingRequest_get();
    // const bool missing_hb          = app_heartbeatMonitor_isSendingMissingHeartbeatFault(&hb_monitor);

    // ONLY RUN THIS WHEN CELLS HAVE HAD TIME TO SETTLE
    // if (app_canRx_Debug_ResetSoc_MinCellV_get())
    // {
    //     app_soc_resetSocFromVoltage();
    // }
    // else if (app_canRx_Debug_ResetSoc_CustomEnable_get())
    // {
    //     app_soc_resetSocCustomValue(app_canRx_Debug_ResetSoc_CustomVal_get());
    // }

    if (irs_negative_closed && ts_discharged)
    {
        const bool external_charging_request = app_canRx_Debug_StartCharging_get();

        if (external_charging_request)
        {
            app_stateMachine_setNextState(&precharge_charge_state);
        }
        // else if (!is_charger_connected)
        // {
        //     // TODO: Precharge for driving!
        // }
        else if (cell_balancing_enabled)
        {
            app_stateMachine_setNextState(&balancing_state);
        }
    }

    // const bool precharge_for_charging = charger_connected && external_charging_request;
    // TODO: Update state change condition
    // const bool precharge_for_driving = !charger_connected && !cell_balancing_enabled && !missing_hb;
    const bool precharge_for_driving = app_canRx_VC_State_get() == VC_BMS_ON_STATE;

    // if (precharge_for_charging)
    // {
    //     app_stateMachine_setNextState(app_prechargeState_get());
    // }
    if (precharge_for_driving)
    {
        app_stateMachine_setNextState(&precharge_state);
    }
    else if (cell_balancing_enabled)
    {
        app_stateMachine_setNextState(&balancing_state);
    }
}

const State init_state = {
    .name              = "INIT",
    .run_on_entry      = initStateRunOnEntry,
    .run_on_tick_100Hz = initStateRunOnTick100Hz,
    .run_on_exit       = NULL,
};