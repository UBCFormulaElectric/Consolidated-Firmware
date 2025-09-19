#include "app_warningHandling.h"

#include "app_signal.h"

#include "app_canAlerts.h"
#include "app_canRx.h"
#include "app_canTx.h"

#include <stdbool.h>
#include <stdint.h>

#define APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT (10u)
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR (10u)

static Signal apps_brake_disagreement_signal;
typedef enum
{
    INVERTER_FL,
    INVERTER_FR,
    INVERTER_RL,
    INVERTER_RR,
    NUM_INVERTERS
} InverterConfig;

bool app_warningHandling_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

typedef struct
{
    void (*can_enable_inv)(bool);
    void (*can_invOn)(bool);
    void (*can_dcOn)(bool);
    uint32_t (*can_error_info)(void);
    void (*error_reset)(bool);
} InverterWarningHandling;

static InverterWarningHandling inverter_reset_handle[NUM_INVERTERS] = {
    [INVERTER_FL] = { .can_enable_inv = app_canTx_VC_INVFLbEnable_set,
                      .can_invOn      = app_canTx_VC_INVFLbInverterOn_set,
                      .can_dcOn       = app_canTx_VC_INVFLbDcOn_set,
                      .can_error_info = app_canRx_INVFL_ErrorInfo_get,
                      .error_reset    = app_canTx_VC_INVFLbErrorReset_set },
    [INVERTER_FR] = { .can_enable_inv = app_canTx_VC_INVFRbEnable_set,
                      .can_invOn      = app_canTx_VC_INVFRbInverterOn_set,
                      .can_dcOn       = app_canTx_VC_INVFRbDcOn_set,
                      .can_error_info = app_canRx_INVFR_ErrorInfo_get,
                      .error_reset    = app_canTx_VC_INVFRbErrorReset_set },
    [INVERTER_RL] = { .can_enable_inv = app_canTx_VC_INVRLbEnable_set,
                      .can_invOn      = app_canTx_VC_INVRLbInverterOn_set,
                      .can_dcOn       = app_canTx_VC_INVRLbDcOn_set,
                      .can_error_info = app_canRx_INVRL_ErrorInfo_get,
                      .error_reset    = app_canTx_VC_INVRLbErrorReset_set },
    [INVERTER_RR] = { .can_enable_inv = app_canTx_VC_INVRRbEnable_set,
                      .can_invOn      = app_canTx_VC_INVRRbInverterOn_set,
                      .can_dcOn       = app_canTx_VC_INVRRbDcOn_set,
                      .can_error_info = app_canRx_INVRR_ErrorInfo_get,
                      .error_reset    = app_canTx_VC_INVRRbErrorReset_set },
};

bool app_warningHandling_inverterStatus(void)
{
    const bool invrr_error = app_canRx_INVRR_bError_get() == true;
    const bool invrl_error = app_canRx_INVRL_bError_get() == true;
    const bool invfl_error = app_canRx_INVFL_bError_get() == true;
    const bool invfr_error = app_canRx_INVFR_bError_get() == true;

    app_canAlerts_VC_Warning_RearLeftInverterFault_set(invrl_error);
    app_canAlerts_VC_Warning_FrontRightInverterFault_set(invrr_error);
    app_canAlerts_VC_Warning_FrontLeftInverterFault_set(invfl_error);
    app_canAlerts_VC_Warning_RearRightInverterFault_set(invfr_error);

    return invfl_error || invrl_error || invrr_error || invfr_error;
}

// TODO: integrate with warnings (will have to make compatible with regen)
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage)
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

void app_warningHandling_inverterReset(void)
{
    for (uint8_t inverter = 0; inverter < NUM_INVERTERS; inverter++)
    {
        inverter_reset_handle[inverter].can_invOn(false);
        inverter_reset_handle[inverter].can_dcOn(false);
        inverter_reset_handle[inverter].can_enable_inv(false);
        inverter_reset_handle[inverter].error_reset(true);
    }
}
void app_softwareBspd_init(void)
{
    app_signal_init(
        &apps_brake_disagreement_signal, APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT, APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR);
}
