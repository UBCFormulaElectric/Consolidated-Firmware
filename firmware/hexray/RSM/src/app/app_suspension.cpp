#include "app_suspension.hpp"
#include "io_suspension.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

namespace app::suspension
{
void broadcast()
{
    const float left_travel  = io::suspension::getRLTravel();
    const float right_travel = io::suspension::getRRTravel();
    can_alerts::infos::RearRightSuspension_OCSC_set(io::suspension::RR_OCSC());
    can_alerts::infos::RearLeftSuspension_OCSC_set(io::suspension::RL_OCSC());
    can_tx::RSM_RearLeftSuspensionTravel_set(left_travel);
    can_tx::RSM_RearRightSuspensionTravel_set(right_travel);
}
} // namespace app::suspension