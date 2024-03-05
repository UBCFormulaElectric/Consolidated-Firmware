#include <math.h>
#include <stddef.h>
#include <stdbool.h>

#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "app_powerManager.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canUtils.h"

static void initStateRunOnEntry(void)
{
    app_allStates_runOnTick100Hz();
    app_canTx_PDM_State_set(PDM_INIT_STATE);
    app_powerManager_setState(POWER_MANAGER_SHUTDOWN);
}

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    bool is_inverter_on_or_drive_state = app_canRx_BMS_State_get() == BMS_INVERTER_ON_STATE ||
                                         app_canRx_BMS_State_get() == BMS_PRECHARGE_STATE ||
                                         app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    if (is_inverter_on_or_drive_state)
    {
        app_stateMachine_setNextState(app_driveState_get());
    }

    app_powerManager_check_efuses(POWER_MANAGER_SHUTDOWN);
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
