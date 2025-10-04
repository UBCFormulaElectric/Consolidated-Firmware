#include "app_suspension.hpp"
#include "io_suspension.hpp"
// #include "app_canTx.hpp"
// #include "app_canAlerts.hpp" //Not created yet

void app_canAlerts_RSM_Info_RearRightSuspension_OCSC_set(int);
void app_canAlerts_RSM_Info_RearLeftSuspension_OCSC_set(int);
void app_canTx_RSM_RearLeftSuspensionTravel_set(int);
void app_canTx_RSM_RearRightSuspensionTravel_set(int);

void app_suspension_broadcast()
{
    const float left_travel = io_suspension_getRLTravel();
    const float right_travel = io_suspension_getRRTravel();
    app_canAlerts_RSM_Info_RearRightSuspension_OCSC_set(io_suspension_rr_OCSC());
    app_canAlerts_RSM_Info_RearLeftSuspension_OCSC_set(io_suspension_rl_OCSC());
    app_canTx_RSM_RearLeftSuspensionTravel_set(left_travel);
    app_canTx_RSM_RearRightSuspensionTravel_set(right_travel);
}