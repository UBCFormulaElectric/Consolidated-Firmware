#include "io_suspension.h"
#include "app_suspension.h"

void app_suspension_broadcast(void)
{
    float left_travel  = io_suspension_getrlTravel();
    float right_travel = io_suspension_getrrTravel();

    app_canTx_RSM_RearLeftSuspensionTravel_set(left_travel);
    app_canTx_RSM_RearRightSuspensionTravel_set(right_travel);
}
