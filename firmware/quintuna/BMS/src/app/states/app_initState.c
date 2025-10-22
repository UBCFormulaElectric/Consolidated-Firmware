#include "states/app_states.h"

#include "app_canRx.h"
#include "app_canTx.h"
#include "app_tractiveSystem.h"
#include "app_canUtils.h"
// #include "app_soc.h"

#include "io_charger.h"
#include "io_irs.h"

#include <stddef.h>

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void initStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INIT_STATE);
    // AIR+ opens upon entering init state
    // Should always be opened at this point from other states, this is only for redundancy since we really don't want
    // AIR+ closed in init
    io_irs_setPositive(CONTACTOR_STATE_OPEN);
}

static void initStateRunOnTick100Hz(void)
{
    const bool irs_negative_closed = io_irs_negativeState() == CONTACTOR_STATE_CLOSED;
    const bool ts_discharged       = app_tractiveSystem_getVoltage() < TS_DISCHARGED_THRESHOLD_V;

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
        const bool charger_connected         = io_charger_getConnectionStatus() == CHARGER_CONNECTED_WALL ||
                                       io_charger_getConnectionStatus() == CHARGER_CONNECTED_EVSE;
        const bool precharge_for_driving  = app_canRx_VC_State_get() == VC_BMS_ON_STATE;
        const bool cell_balancing_enabled = app_canRx_Debug_CellBalancingRequest_get();

        if (external_charging_request && charger_connected)
        {
            app_stateMachine_setNextState(&precharge_charge_state);
        }
        else if (precharge_for_driving)
        {
            app_stateMachine_setNextState(&precharge_drive_state);
        }
        else if (cell_balancing_enabled)
        {
            app_stateMachine_setNextState(&balancing_state);
        }
    }
}

const State init_state = {
    .name              = "INIT",
    .run_on_entry      = initStateRunOnEntry,
    .run_on_tick_100Hz = initStateRunOnTick100Hz,
    .run_on_exit       = NULL,
};