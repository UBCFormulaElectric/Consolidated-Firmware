#include "app_warningHandling.h"
#include "app_canUtils.h"
#include <app_canRx.h>
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include <stdint.h>
#include "app_stateMachine.h"

typedef enum
{
    INVERTER_FL,
    INVERTER_FR,
    INVERTER_RL,
    INVERTER_RR,
    NUM_INVERTERS
} InverterConfig;

static bool app_warningHandling_boardWarningCheck(void)
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
    app_canAlerts_VC_Warning_RearLeftInverterFault_set(invfl_error);
    app_canAlerts_VC_Warning_RearRightInverterFault_set(invfr_error);

    return invfl_error || invrl_error || invfl_error || invfr_error;
}

WarningType app_warningHandling_globalWarningCheck()
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
