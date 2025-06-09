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

bool app_faultHandling_bmsLatchedFaults(void)
{
    return app_canAlerts_BoardHasFault(BMS_NODE);
}

bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
{
    return app_canAlerts_BoardHasFault(VC_NODE); 
}