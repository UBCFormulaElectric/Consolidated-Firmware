#include <math.h>
#include "states/app_initState.h"
#include "states/app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_powerManager.h"
#include <stddef.h>
#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_buzzer.h"
#include "app_globals.h"

static void initStateRunOnEntry(void)
{
    app_allStates_runOnTick100Hz();
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_setState(POWER_MANAGER_SHUTDOWN);
    // Disable inverters and apply zero torque upon entering init state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);

    // Disable buzzer on transition to init.
    io_buzzer_enable(false);
    app_canTx_VC_BuzzerOn_set(false);
}

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    bool is_inverter_on_or_drive_state = app_canRx_BMS_State_get() == BMS_INVERTER_ON_STATE ||
                                         app_canRx_BMS_State_get() == BMS_PRECHARGE_STATE ||
                                         app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    if (is_inverter_on_or_drive_state)
    {
        app_stateMachine_setNextState(app_inverterOnState_get());
    }
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
