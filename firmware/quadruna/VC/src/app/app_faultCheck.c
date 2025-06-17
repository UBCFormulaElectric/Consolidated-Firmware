#include "app_faultCheck.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_signal.h"

static Signal apps_brake_disagreement_signal;

void app_faultCheck_init(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT, APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR);
}

bool app_faultCheck_checkBoards(void)
{
    const bool bms_fault  = app_canAlerts_BoardHasFault(BMS_ALERT_BOARD);
    const bool vc_fault   = app_canAlerts_BoardHasFault(VC_ALERT_BOARD);
    const bool fsm_fault  = app_canAlerts_BoardHasFault(FSM_ALERT_BOARD);
    const bool crit_fault = app_canAlerts_BoardHasFault(CRIT_ALERT_BOARD);

    return (bms_fault || vc_fault || fsm_fault || crit_fault);
}

bool app_faultCheck_checkInverters()
{
    const bool left_inverter_fault  = app_canRx_INVL_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    const bool right_inverter_fault = app_canRx_INVR_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    app_canAlerts_VC_Fault_LeftInverterFault_set(left_inverter_fault);
    app_canAlerts_VC_Fault_RightInverterFault_set(right_inverter_fault);

    return (left_inverter_fault || right_inverter_fault);
}

bool app_faultCheck_checkSoftwareBspd(const float papps_pedal_percentage, const float sapps_pedal_percentage)
{
    // Accelerator Brake Plausibility (bad user input safety issues)
    // Protect against brake/apps active at same time
    // Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
    // Allowed to exit disagreement only when apps is released (< 5%)
    bool apps_brakes_conflict =
        app_canRx_FSM_BrakeActuated_get() && (papps_pedal_percentage > 0.25f || sapps_pedal_percentage > 0.25f);

    bool apps_less_than_5_percent = papps_pedal_percentage < 0.05f && sapps_pedal_percentage < 0.05f;

    const SignalState apps_brake_disagreement_signal_state =
        app_signal_getState(&apps_brake_disagreement_signal, apps_brakes_conflict, apps_less_than_5_percent);

    const bool apps_brake_disagreement_active = apps_brake_disagreement_signal_state == SIGNAL_STATE_ACTIVE;

    app_canAlerts_VC_Warning_BrakeAppsDisagreement_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}