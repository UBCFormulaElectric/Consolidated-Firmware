#include "app_allStates.h"
#include "app_driveState.h"

#include "app_utils.h"

#include "io_airs.h"
#include "io_faultLatch.h"
#include "io_airs.h"

static void driveWarningStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(VC_DRIVE_WARNING_STATE);
    app_canAlerts_VC_Fault_StateMachine_set(true);

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

static void driveWarningStateRunOnExit(void)
{
    app_canAlerts_BMS_Fault_StateMachine_set(false);
}

const State *app_driveWarningState_get()
{
    static State driveWarning_state = {
        .name              = "DRIVE_WARNING",
        .run_on_entry      = driveWarningStateRunOnEntry,
        .run_on_tick_1Hz   = driveWarningStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveWarningStateRunOnTick100Hz,
        .run_on_exit       = driveWarningStateRunOnExit,
    };

    return &driveWarning_state;
}
