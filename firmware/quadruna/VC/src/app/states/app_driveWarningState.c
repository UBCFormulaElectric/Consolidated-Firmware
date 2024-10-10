#include "app_allStates.h"
#include "app_driveState.h"

#include "app_utils.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

static void driveWarningStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_DRIVE_WARNING_STATE);

    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
}

static void driveWarningStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void driveWarningStateRunOnTick100Hz(void)
{
    const bool vc_has_warning   = app_canAlerts_BoardHasWarning(VC_ALERT_BOARD);
    const bool bms_has_warning  = app_canAlerts_BoardHasWarning(BMS_ALERT_BOARD);
    const bool fsm_has_warning  = app_canAlerts_BoardHasWarning(FSM_ALERT_BOARD);
    const bool crit_has_warning = app_canAlerts_BoardHasWarning(CRIT_ALERT_BOARD);
    const bool rsm_has_warning  = app_canAlerts_BoardHasWarning(RSM_ALERT_BOARD);

    const bool is_board_in_warning_state =
        vc_has_warning || bms_has_warning || fsm_has_warning || crit_has_warning || rsm_has_warning;

    if (app_allStates_runOnTick100Hz())
    {
        if (!is_board_in_warning_state)
        {
            app_stateMachine_setNextState(app_driveState_get());
        }
    }
}

const State *app_driveWarningState_get()
{
    static State driveWarning_state = {
        .name              = "DRIVE_WARNING",
        .run_on_entry      = driveWarningStateRunOnEntry,
        .run_on_tick_1Hz   = driveWarningStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveWarningStateRunOnTick100Hz,
    };

    return &driveWarning_state;
}
