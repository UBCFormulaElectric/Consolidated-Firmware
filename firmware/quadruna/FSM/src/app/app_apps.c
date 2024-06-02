#include "app_apps.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "app_signal.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_apps.h"
#include "io_brake.h"

Signal papps_ocsc_signal;
Signal sapps_ocsc_signal;
Signal papps_sapps_disagreement_signal;

void app_apps_init(void)
{
    app_signal_init(&papps_ocsc_signal, APPS_OCSC_TIME_TO_FAULT, APPS_OCSC_TIME_TO_CLEAR);
    app_signal_init(&sapps_ocsc_signal, APPS_OCSC_TIME_TO_FAULT, APPS_OCSC_TIME_TO_CLEAR);
    app_signal_init(
        &papps_sapps_disagreement_signal, PAPPS_SAPPS_DISAGREEMENT_TIME_TO_FAULT,
        PAPPS_SAPPS_DISAGREEMENT_TIME_TO_CLEAR);
}

void app_apps_broadcast()
{
    float papps_pedal_percentage = io_apps_getPrimary();
    float sapps_pedal_percentage = io_apps_getSecondary();

    // Open Short Circuit Tests (pin voltages out of bounds)
    const bool  primary_pedal_ocsc = io_apps_isPrimaryOCSC();
    SignalState papps_ocsc_signal_state =
        app_signal_getState(&papps_ocsc_signal, primary_pedal_ocsc, !primary_pedal_ocsc);
    const bool papps_ocsc_active = papps_ocsc_signal_state == SIGNAL_STATE_ACTIVE;

    const bool  secondary_pedal_ocsc = io_apps_isSecondaryOCSC();
    SignalState sapps_ocsc_signal_state =
        app_signal_getState(&sapps_ocsc_signal, secondary_pedal_ocsc, !secondary_pedal_ocsc);
    const bool sapps_ocsc_active = sapps_ocsc_signal_state == SIGNAL_STATE_ACTIVE;

    app_canAlerts_FSM_Fault_PappsOCSC_set(papps_ocsc_active);
    app_canAlerts_FSM_Fault_SappsOCSC_set(sapps_ocsc_active);

    // Primary Secondary Accelerator Agreement (Inaccurate data)
    const float papps_sapps_diff = fabsf(papps_pedal_percentage - sapps_pedal_percentage);

    SignalState papps_sapps_disagreement_signal_state =
        app_signal_getState(&papps_sapps_disagreement_signal, (papps_sapps_diff) > 10.f, (papps_sapps_diff) <= 10.f);

    const bool papps_sapps_disagreement_active = papps_sapps_disagreement_signal_state == SIGNAL_STATE_ACTIVE;

    app_canAlerts_FSM_Warning_AppsDisagreement_set(papps_sapps_disagreement_active);

    app_canTx_FSM_PappsRawPedalPercentage_set(papps_pedal_percentage);
    app_canTx_FSM_SappsRawPedalPercentage_set(sapps_pedal_percentage);

    // set mapped apps to 0 if anything went wrong
    if (papps_ocsc_active || sapps_ocsc_active || papps_sapps_disagreement_active)
    {
        app_canTx_FSM_PappsMappedPedalPercentage_set(0.0f);
        app_canTx_FSM_SappsMappedPedalPercentage_set(0.0f);
    }
    else
    {
        app_canTx_FSM_PappsMappedPedalPercentage_set(papps_pedal_percentage);
        app_canTx_FSM_SappsMappedPedalPercentage_set(sapps_pedal_percentage);
    }
}
