#include "app_warningHanding.h"
#include "app_canUtils.h"
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include "app_stateMachine.h"


static bool app_warningHandling_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

static bool app_warningHandling_inverterStatus(void)
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
