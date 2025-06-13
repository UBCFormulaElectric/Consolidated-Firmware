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