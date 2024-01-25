#include "states/app_allStates.h"
#include "App_SharedMacros.h"
#include "app_imd.h"
#include "io_faultLatch.h"
#include "io_airs.h"
#include "app_inverterOnState.h"
#include "app_soc.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void initStateRunOnEntry(struct StateMachine* const state_machine)
{
    App_CanTx_BMS_State_Set(BMS_INIT_STATE);
    app_accumulator_writeDefaultConfig();
    io_faultLatch_setCurrentStatus(globals->config->bms_ok_latch, true);

    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_airs_openPositive();
}

static void initStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);

    // ONLY RUN THIS WHEN CELLS HAVE HAD TIME TO SETTLE
    if (App_CanRx_Debug_ResetSoc_MinCellV_Get())
    {
        app_soc_resetSocFromVoltage();
    }
    else if (App_CanRx_Debug_ResetSoc_CustomEnable_Get())
    {
        app_soc_resetSocCustomValue(App_CanRx_Debug_ResetSoc_CustomVal_Get());
    }
}

static void initStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        const bool air_negative_closed = io_airs_isNegativeClosed();
        const bool ts_discharged       = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;

        if (air_negative_closed && ts_discharged)
        {
            const bool charger_connected         = io_charger_isConnected();
            const bool cell_balancing_enabled    = App_CanRx_Debug_CellBalancingRequest_Get();
            const bool external_charging_request = App_CanRx_Debug_StartCharging_Get();

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
                App_SharedStateMachine_SetNextState(state_machine, app_prechargeState_get());
            }
            else if (precharge_for_driving)
            {
                App_SharedStateMachine_SetNextState(state_machine, app_inverterOnState_get());
            }
            else if (cell_balancing_enabled)
            {
                App_SharedStateMachine_SetNextState(state_machine, app_balancingState_get());
            }
#endif
        }
    }
}

static void initStateRunOnExit(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

const struct State* app_initState_get(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = initStateRunOnExit,
    };

    return &init_state;
}
