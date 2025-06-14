#include "app_warningHanding.h"
#include "app_canUtils.h"
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include "app_stateMachine.h"
#include "app_signal.h"

static Signal apps_brake_disagreement_signal;

static bool app_warningHandling_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

bool app_warningHandling_inverterStatus(void)
{
    const bool invrr_error = app_canRx_INVRR_bError_get() == true;
    const bool invrl_error = app_canRx_INVRL_bError_get() == true;
    const bool invfl_error = app_canRx_INVFL_bError_get() == true;
    const bool invfr_error = app_canRx_INVFR_bError_get() == true;

    app_canAlerts_VC_Warning_RearLeftInverterFault_set(invrl_error);
    app_canAlerts_VC_Warning_FrontRightInverterFault_set(invrr_error);
    app_canAlerts_VC_Warning_RearLeftInverterFault_set(invfl_error);
    app_canAlerts_VC_Warning_RearRightInverterFault_set(invfr_error);

    return invfl_error || invrl_error || invfl_error || invfr_error;
}

// TODO: integrate with warnings (will have to make compatible with regen)
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage, float sapps_pedal_percentage)
{
    // Accelerator Brake Plausibility (bad user input safety issues)
    // Protect against brake/apps active at same time
    // Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
    // Allowed to exit disagreement only when apps is released (< 5%)
    bool apps_brakes_conflict =
        app_canRx_FSM_BrakeActuated_get() && (papps_pedal_percentage > 0.25f || sapps_pedal_percentage > 0.25f);

    bool apps_less_than_5_percent = papps_pedal_percentage < 0.05f && sapps_pedal_percentage < 0.05f;

    SignalState apps_brake_disagreement_signal_state =
        app_signal_getState(&apps_brake_disagreement_signal, apps_brakes_conflict, apps_less_than_5_percent);

    const bool apps_brake_disagreement_active = apps_brake_disagreement_signal_state == SIGNAL_STATE_ACTIVE;

    app_canAlerts_VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

warningType app_warningHandling_globalWarningCheck()
{
    if (app_warningHandling_inverterStatus())
    {
        return INVERTER_FAULT;
    }
    else if (app_warningHandling_boardWarningCheck())
    {
        return BOARD_WARNING_DETECTED;
    }

    return NO_WARNINGS;
}

void app_softwareBspd_init(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT, APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR);
}