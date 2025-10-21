#include "app/suspension.h"

#include "io/suspension.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

void app_suspension_broadcast(void)
{
    const float left_travel  = io_suspension_getRLTravel();
    const float right_travel = io_suspension_getRRTravel();
    app_canAlerts_RSM_Info_RearRightSuspension_OCSC_set(io_suspension_rr_OCSC());
    app_canAlerts_RSM_Info_RearLeftSuspension_OCSC_set(io_suspension_rl_OCSC());
    app_canTx_RSM_RearLeftSuspensionTravel_set(left_travel);
    app_canTx_RSM_RearRightSuspensionTravel_set(right_travel);
}
