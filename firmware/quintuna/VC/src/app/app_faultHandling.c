#include "app_faultHandling.h"
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_signal.h"

#define NUM_FAULT_NODES 14

static Signal apps_brake_disagreement_signal;

/**
 * New fault handling will not kick drive out of drive state, instead it will clamp torque to 0, this done to reduce
 * exiting drive state on recoverable faults
 */

bool app_faultHandling_air_minus_closed(void)
{
    return (CONTACTOR_STATE_CLOSED == app_canRx_BMS_IrNegative_get());
}

bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
{
    return app_canAlerts_BoardHasFault(VC_NODE);
}

void app_faultCheck_softwareBspdInit(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT, APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR);
}

bool app_faultCheck_checkSoftwareBspd(float papps_pedal_percentage, float sapps_pedal_percentage)
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

    app_canAlerts_VC_Warning_BrakeAppsDisagreement_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}