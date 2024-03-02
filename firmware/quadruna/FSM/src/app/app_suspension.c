#include "app_suspension.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_suspension.h"

void app_suspension_broadcast(void)
{
    float left_travel  = io_suspension_getLeftTravel();
    float right_travel = io_suspension_getRightTravel();

    app_canTx_FSM_LeftSuspensionTravel_set(left_travel);
    app_canTx_FSM_LeftSuspensionTravel_set(right_travel);

    app_canAlerts_FSM_Warning_LeftSuspensionOCSC_set(io_suspension_leftSensorOCSC());
    app_canAlerts_FSM_Warning_RightSuspensionOCSC_set(io_suspension_rightSensorOCSC());
}