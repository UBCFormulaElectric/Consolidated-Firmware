#include "app/faultHandling.h"
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <app_canAlerts.h>
#include "app/stateMachine.h"
#include "app/states/states.h"

#define NUM_FAULT_NODES 14

bool app_faultHandling_air_minus_closed(void)
{
    return (CONTACTOR_STATE_CLOSED == app_canRx_BMS_IrNegative_get());
}

bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
{
    return app_canAlerts_BoardHasFault(VC_NODE);
}
