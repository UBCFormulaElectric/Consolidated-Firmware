#include "app_warningHandling.hpp"

constexpr uint32_t app_brake_diagreement_time_to_fault = 10u;
constexpr uint32_t app_brake_diagreement_time_to_clear = 10u;

static Signal apps_brake_disagreement_signal;

// TODO: integrate with warnings (will have to make compatible with regen)
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage)
{
    // Accelerator Brake System Plausibility device (bad user input safety issues)
    // Protect against brake/apps active at same time
    // Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
    // Allowed to exit disagreement only when apps is released (< 5%)
    const bool apps_brakes_conflict = app_canRx_FSM_BrakeActuated_get() && (papps_pedal_percentage > 0.25f);

    const bool apps_less_than_5_percent = (papps_pedal_percentage < 0.05f);

    const SignalState apps_brake_disagreement_signal_state =
        app_signal_getState(&apps_brake_disagreement_signal, apps_brakes_conflict, apps_less_than_5_percent);

    const bool apps_brake_disagreement_active = (apps_brake_disagreement_signal_state == SIGNAL_STATE_ACTIVE);

    app_canAlerts_VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

void app_softwareBspd_init(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, app_brake_diagreement_time_to_fault, app_brake_diagreement_time_to_clear);
}
