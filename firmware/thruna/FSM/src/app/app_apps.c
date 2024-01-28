#include "app_apps.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "app_signal.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_apps.h"
#include "io_brake.h"

Signal app_agreement_signal;
Signal papp_alarm_signal;
Signal sapp_alarm_signal;
Signal app_brake_signal;

void app_apps_init(void)
{
    app_signal_init(&app_agreement_signal, AGREEMENT_TIME_TO_FAULT, AGREEMENT_TIME_TO_CLEAR);
    app_signal_init(&papp_alarm_signal, PAPPS_OCSC_TIME_TO_FAULT, PAPPS_OCSC_TIME_TO_CLEAR);
    app_signal_init(&sapp_alarm_signal, SAPPS_OCSC_TIME_TO_FAULT, SAPPS_OCSC_TIME_TO_CLEAR);
    app_signal_init(&app_brake_signal, APP_BRAKE_TIME_TO_FAULT, APP_BRAKE_TIME_TO_CLEAR);
}

void app_apps_broadcast()
{
    float papps_pedal_percentage = io_apps_getPrimary();
    float sapps_pedal_percentage = io_apps_getSecondary();
    app_canTx_FSM_PappsMappedPedalPercentage_set(papps_pedal_percentage);
    app_canTx_FSM_SappsMappedPedalPercentage_set(sapps_pedal_percentage);
    app_canTx_FSM_PappsRawPedalPercentage_set(papps_pedal_percentage);
    app_canTx_FSM_SappsRawPedalPercentage_set(sapps_pedal_percentage);

    // Open Short Circuit Tests (non-understandable data test)
    const bool  primary_pedal_ocsc = io_apps_isPrimaryOCSC();
    SignalState papp_signal_state  = app_signal_getState(&papp_alarm_signal, primary_pedal_ocsc, !primary_pedal_ocsc);
    const bool  papps_ocsc_active  = papp_signal_state == SIGNAL_STATE_ACTIVE;
    app_canAlerts_FSM_Fault_PappsOCSC_set(papps_ocsc_active);

    const bool  secondary_pedal_ocsc = io_apps_isSecondaryOCSC();
    SignalState sapp_signal_state =
        app_signal_getState(&sapp_alarm_signal, secondary_pedal_ocsc, !secondary_pedal_ocsc);
    const bool sapps_ocsc_active = sapp_signal_state == SIGNAL_STATE_ACTIVE;
    app_canAlerts_FSM_Fault_SappsOCSC_set(sapps_ocsc_active);

    // torque 0
    if (papps_ocsc_active || sapps_ocsc_active)
    {
        app_canTx_FSM_PappsMappedPedalPercentage_set(0.0f);
        app_canTx_FSM_SappsMappedPedalPercentage_set(0.0f);
    }

    // Primary Secondary Accelerator Agreement (Inaccurate data)
    const float papp_sapp_diff = fabsf(papps_pedal_percentage - sapps_pedal_percentage);
    SignalState app_agreement_signal_state =
        app_signal_getState(&app_agreement_signal, (papp_sapp_diff) > 10.f, (papp_sapp_diff) <= 10.f);
    const bool apps_disagreement = app_agreement_signal_state == SIGNAL_STATE_ACTIVE;
    app_canAlerts_FSM_Warning_AppsDisagreement_set(apps_disagreement);

    if (apps_disagreement)
    {
        app_canTx_FSM_PappsMappedPedalPercentage_set(0.0f);
        app_canTx_FSM_SappsMappedPedalPercentage_set(0.0f);
    }

    // Accelerator Brake Plausibility (bad user input safety issues)
    SignalState app_brake_disagreement = app_signal_getState(
        &app_brake_signal, io_brake_isActuated() && (papps_pedal_percentage > 25 || papps_pedal_percentage > 25),
        papps_pedal_percentage < 5);
    const bool brake_acc_disagreement = app_brake_disagreement == SIGNAL_STATE_ACTIVE;
    app_canAlerts_FSM_Warning_BrakeAppsDisagreement_set(brake_acc_disagreement);

    if (brake_acc_disagreement)
    {
        app_canTx_FSM_PappsMappedPedalPercentage_set(0.0f);
        app_canTx_FSM_SappsMappedPedalPercentage_set(0.0f);
    }
}
