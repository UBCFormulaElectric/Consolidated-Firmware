
#include "app_warningHandling.h"
#include "app_signal.h"
#include "app_canAlerts.h"
#include "app_canRx.h"
#include "app_canTx.h"
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT (10u)
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR (10u)

static Signal apps_brake_disagreement_signal;

bool app_warningHandlin_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

// TODO: integrate with warnings (will have to make compatible with regen)
bool app_warningHandlin_checkSoftwareBspd(float papps_pedal_percentage)
{
    // Accelerator Brake Plausibility (bad user input safety issues)
    // Protect against brake/apps active at same time
    // Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
    // Allowed to exit disagreement only when apps is released (< 5%)
    bool apps_brakes_conflict = app_canRx_FSM_BrakeActuated_get() && (papps_pedal_percentage > 0.25f);

    bool apps_less_than_5_percent = papps_pedal_percentage < 0.05f;

    SignalState apps_brake_disagreement_signal_state =
        app_signal_getState(&apps_brake_disagreement_signal, apps_brakes_conflict, apps_less_than_5_percent);

    const bool apps_brake_disagreement_active = apps_brake_disagreement_signal_state == SIGNAL_STATE_ACTIVE;

    app_canAlerts_VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

void app_softwareBspd_init(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT, APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR);
}
