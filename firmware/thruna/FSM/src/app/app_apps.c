#include "app_apps.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "App_SharedSignal.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "io_apps.h"
#include "io_brake.h"

struct Signal* app_agreement_signal;
struct Signal* papp_alarm_signal;
struct Signal* sapp_alarm_signal;
struct Signal* app_brake_signal;

void app_apps_init(void)
{
    app_agreement_signal = App_SharedSignal_Create(AGREEMENT_TIME_TO_FAULT, AGREEMENT_TIME_TO_CLEAR);
    papp_alarm_signal    = App_SharedSignal_Create(PAPPS_OCSC_TIME_TO_FAULT, PAPPS_OCSC_TIME_TO_CLEAR);
    sapp_alarm_signal    = App_SharedSignal_Create(SAPPS_OCSC_TIME_TO_FAULT, SAPPS_OCSC_TIME_TO_CLEAR);
    app_brake_signal     = App_SharedSignal_Create(APP_BRAKE_TIME_TO_FAULT, APP_BRAKE_TIME_TO_CLEAR);
}

void app_apps_destroy()
{
    App_SharedSignal_Destroy(app_agreement_signal);
    App_SharedSignal_Destroy(papp_alarm_signal);
    App_SharedSignal_Destroy(sapp_alarm_signal);
    App_SharedSignal_Destroy(app_brake_signal);
}

void app_apps_broadcast()
{
    float papps_pedal_percentage = io_apps_getPrimary();
    float sapps_pedal_percentage = io_apps_getSecondary();
    App_CanTx_FSM_PappsMappedPedalPercentage_Set(papps_pedal_percentage);
    App_CanTx_FSM_SappsMappedPedalPercentage_Set(sapps_pedal_percentage);
    App_CanTx_FSM_PappsRawPedalPercentage_Set(papps_pedal_percentage);
    App_CanTx_FSM_SappsRawPedalPercentage_Set(sapps_pedal_percentage);

    // Open Short Circuit Tests (non-understandable data test)
    const bool  primary_pedal_ocsc = io_apps_isPrimaryOCSC();
    SignalState papp_signal_state = App_SharedSignal_Update(papp_alarm_signal, primary_pedal_ocsc, !primary_pedal_ocsc);
    const bool  papps_ocsc_active = papp_signal_state == SIGNAL_STATE_ACTIVE;
    App_CanAlerts_FSM_Fault_PappsOCSC_Set(papps_ocsc_active);

    const bool  secondary_pedal_ocsc = io_apps_isSecondaryOCSC();
    SignalState sapp_signal_state =
        App_SharedSignal_Update(sapp_alarm_signal, secondary_pedal_ocsc, !secondary_pedal_ocsc);
    const bool sapps_ocsc_active = sapp_signal_state == SIGNAL_STATE_ACTIVE;
    App_CanAlerts_FSM_Fault_SappsOCSC_Set(sapps_ocsc_active);

    // torque 0
    if (papps_ocsc_active || sapps_ocsc_active)
    {
        App_CanTx_FSM_PappsMappedPedalPercentage_Set(0.0f);
        App_CanTx_FSM_SappsMappedPedalPercentage_Set(0.0f);
    }

    // Primary Secondary Accelerator Agreement (Inaccurate data)
    const float papp_sapp_diff = fabsf(papps_pedal_percentage - sapps_pedal_percentage);
    SignalState app_agreement_signal_state =
        App_SharedSignal_Update(app_agreement_signal, (papp_sapp_diff) > 10.f, (papp_sapp_diff) <= 10.f);
    const bool apps_disagreement = app_agreement_signal_state == SIGNAL_STATE_ACTIVE;
    App_CanAlerts_FSM_Warning_AppsDisagreement_Set(apps_disagreement);

    if (apps_disagreement)
    {
        App_CanTx_FSM_PappsMappedPedalPercentage_Set(0.0f);
        App_CanTx_FSM_SappsMappedPedalPercentage_Set(0.0f);
    }

    // Accelerator Brake Plausibility (bad user input safety issues)
    SignalState app_brake_disagreement = App_SharedSignal_Update(
        app_brake_signal, io_brake_isActuated() && (papps_pedal_percentage > 25 || papps_pedal_percentage > 25),
        papps_pedal_percentage < 5);
    const bool brake_acc_disagreement = app_brake_disagreement == SIGNAL_STATE_ACTIVE;
    App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Set(brake_acc_disagreement);

    if (brake_acc_disagreement)
    {
        App_CanTx_FSM_PappsMappedPedalPercentage_Set(0.0f);
        App_CanTx_FSM_SappsMappedPedalPercentage_Set(0.0f);
    }
}
