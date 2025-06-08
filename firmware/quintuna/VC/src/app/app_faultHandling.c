#include "app_faultHandling.h"
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include "app_stateMachine.h"
#include "states/app_states.h"

#define NUM_FAULT_NODES 14

/**
 * New fault handling will not kick drive out of drive state, instead it will clamp torque to 0, this done to reduce exiting drive state 
 * on recoverable faults 
 */

static bool app_faultHandling_checkBoardStatus(void)
{
    return app_canAlerts_BoardHasFault(BMS_NODE)|| app_canAlerts_BoardHasFault(VC_NODE);
}

static bool app_faultHandling_inverterStatus(void)
{
    const bool invrr_error = app_canRx_INVRR_bError_get() == true;
    const bool invrl_error = app_canRx_INVRL_bError_get() == true;
    const bool invfl_error = app_canRx_INVFL_bError_get() == true;
    const bool invfr_error = app_canRx_INVFR_bError_get() == true;

    app_canAlerts_VC_Fault_RearLeftInverterFault_set(invrl_error);
    app_canAlerts_VC_Fault_RearRightInverterFault_set(invrr_error);
    app_canAlerts_VC_Fault_FrontLeftInverterFault_set(invfl_error);
    app_canAlerts_VC_Fault_FrontRightInverterFault_set(invfr_error);

    return invfl_error || invrl_error || invfl_error || invfr_error;
}

faultType app_faultHandling_globalFaultCheck(void)
{
    if (app_faultHandling_checkBoardStatus())
    {
        return BOARD_FAULT;
    }
    else if (app_faultHandling_inverterStatus())
    {
        return INVERTER_FAULT;
    }
    return NO_FAULT; 
}
