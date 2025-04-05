#include "app_suspension.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_suspension.h"

void app_suspension_broadcast(void)
{
    float left_travel  = io_suspension_getLeftTravel();
    float right_travel = io_suspension_getRightTravel();

    // TODO: send can message with the left and right suspension travel

    // TODO: send can alert if left or right ocsc triggered (set it)
}
