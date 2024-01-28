#include "states/app_allStates.h"
#include "app_utils.h"
#include "app_imd.h"
#include "io_faultLatch.h"
#include "io_airs.h"
#include "app_inverterOnState.h"
#include "app_soc.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void initStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INIT_STATE);
    app_accumulator_writeDefaultConfig();
    io_faultLatch_setCurrentStatus(globals->config->bms_ok_latch, true);

    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_airs_openPositive();
}

static void initStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();

    // ONLY RUN THIS WHEN CELLS HAVE HAD TIME TO SETTLE
    if (app_canRx_Debug_ResetSoc_MinCellV_get())
    {
        app_soc_resetSocFromVoltage();
    }
    else if (app_canRx_Debug_ResetSoc_CustomEnable_get())
    {
        app_soc_resetSocCustomValue(app_canRx_Debug_ResetSoc_CustomVal_get());
    }
}

static void initStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        const bool air_negative_closed = io_airs_isNegativeClosed();
        const bool ts_discharged       = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;

        if (air_negative_closed && ts_discharged)
        {
            const bool charger_connected         = io_charger_isConnected();
            const bool cell_balancing_enabled    = app_canRx_Debug_CellBalancingRequest_get();
            const bool external_charging_request = app_canRx_Debug_StartCharging_get();

            const bool precharge_for_charging = charger_connected && external_charging_request;
            // TODO: Check heartbeat from another board before continuing to precharge? will prevent unintended entry to
            // precharge when preparing to charge
            const bool precharge_for_driving = !charger_connected && !cell_balancing_enabled;

// TODO: Re-implement precharge after remaining testing completed
#ifdef TARGET_EMBEDDED
            (void)precharge_for_charging;
            (void)precharge_for_driving;
            (void)cell_balancing_enabled;
#else
            if (precharge_for_charging)
            {
                app_stateMachine_setNextState(app_prechargeState_get());
            }
            else if (precharge_for_driving)
            {
                app_stateMachine_setNextState(app_inverterOnState_get());
            }
            else if (cell_balancing_enabled)
            {
                app_stateMachine_setNextState(app_balancingState_get());
            }
#endif
        }
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
