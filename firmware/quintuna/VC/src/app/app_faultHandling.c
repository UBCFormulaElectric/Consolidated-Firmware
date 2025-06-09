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

bool app_faultHandling_inverterStatus(void)
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

void app_fault_Handling_clear_inverterFaults(void)
{
        app_canAlerts_VC_Warning_RearLeftInverterFault_set(false);
        app_canAlerts_VC_Warning_RearRightInverterFault_set(false);
        app_canAlerts_VC_Warning_FrontLeftInverterFault_set(false);
        app_canAlerts_VC_Warning_FrontRightInverterFault_set(false);
}

bool app_faultHandling_bmsLatchedFaults(void)
{
    return app_canAlerts_BoardHasFault(BMS_NODE);
}

bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
{
    return app_canAlerts_BoardHasFault(VC_NODE); 
}


faultType app_faultHandling_driveStateFaultChecks(void)
{
    if (app_faultHandling_bmsLatchedFaults())
    {
        return BMS_LATCH_FAULT;
    }
    else if (app_faultHandling_inverterStatus())
    {
        return INVERTER_FAULT;
    }
    else if (app_faultHandling_fsm_bms_HeartbeartChecks)
    {
        return FSM_BSM_HEARTBEAT_FAULT;
    }
    return NO_FAULT; 
}
