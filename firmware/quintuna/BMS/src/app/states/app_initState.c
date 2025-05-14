#include "io_charger.h"
#include "io_faultLatch.h"
#include "io_irs.h"
#include "app_stateMachine.h"
#include "app_allStates.h"

static void app_initStateRunOnEntry()
{
    
    io_charger_init();
    io_faultLatch_getCurrentStatus(&bms_ok_latch, true);

    /**
     * AIR+ opens upon entering init state
     * Should always be opened at this point from other states, this is only for redundancy since we really don't want
     * AIR+ closed in init
     */
    io_irs_openPositive();

    // app_canTx_BMS_State_set(BMS_INIT_STATE);
    // app_accumulator_writeDefaultConfig();
}

static void app_initStateRunOnTick1Hz()
{
    app_allStates_runOnTick1Hz();
    
    // // Only run this when cell have had time to settle
    // if (app_canRx_Debug_ResetSoc_MinCellV_get())
    // {
    //     app_soc_resetSocFromVoltage();
    // }
    // else if (app_canRx_Debug_ResetSoc_CustomEnable_get())
    // {
    //     app_soc_resetSocCustomValue(app_canRx_Debug_ResetSoc_CustomVal_get());
    // }
}

static void app_initStateRunOnTick100Hz()
{
    if(app_allStates_runOnTick100Hz)
    {
        const bool irs_negative_closed = io_irs_isNegativeClosed();
        // const bool ts_discharged       = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;
        // const bool missing_hb          = app_heartbeatMonitor_isSendingMissingHeartbeatFault(&hb_monitor);

        // if (air_negative_closed && ts_discharged)
        // {
        //     const bool charger_connected         = app_canRx_BRUSA_IsConnected_get();
        //     const bool cell_balancing_enabled    = app_canRx_Debug_CellBalancingRequest_get();
        //     const bool external_charging_request = app_canRx_Debug_StartCharging_get();
        //     const bool clear_brusa_latch         = app_canRx_Debug_ClearChargerLatchedFault_get();

        //     app_canTx_BMS_ClearLatch_set(clear_brusa_latch);

        //     const bool precharge_for_charging = charger_connected && external_charging_request;
        //     const bool precharge_for_driving  = !charger_connected && !cell_balancing_enabled && !missing_hb;

        //     if (precharge_for_charging)
        //     {
        //         app_stateMachine_setNextState(app_prechargeState_get());
        //     }
        //     else if (precharge_for_driving)
        //     {
        //         app_stateMachine_setNextState(app_inverterOnState_get());
        //     }
        //     else if (cell_balancing_enabled)
        //     {
        //         app_stateMachine_setNextState(app_balancingState_get());
        //     }
        // }
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