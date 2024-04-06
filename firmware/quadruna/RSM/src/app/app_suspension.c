#include "app_suspension.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_suspension.h"

void app_suspension_broadcast(void)
{
    float const left_travel  = io_suspension_getRearLeftTravel();
    float const right_travel = io_suspension_getRearRightTravel();

    app_canTx_RSM_RearLeftSuspensionTravel_set(left_travel);
    app_canTx_RSM_RearRightSuspensionTravel_set(right_travel);

    app_canAlerts_RSM_Warning_LeftSuspensionOCSC_set(io_suspension_leftSensorOCSC());
    app_canAlerts_RSM_Warning_RightSuspensionOCSC_set(io_suspension_rightSensorOCSC());
}