#include "app_suspension.hpp"
#include "io_suspension.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include <cmath>

namespace app::suspension
{
void broadcast()
{
    constexpr float suspension_min = -10.0f;
    constexpr float suspension_max = 10.0f;

    const float left_travel  = io::suspension::getRLTravel();
    const float right_travel = io::suspension::getRRTravel();
    
    const bool left_travel_Valid = 
        (std::isfinite(left_travel) && (left_travel >= suspension_min) && (left_travel <= suspension_max) && io::suspension::RL_OCSC());

    const bool right_travel_Valid = 
        (std::isfinite(right_travel) && (right_travel >= suspension_min) && (right_travel <= suspension_max) && io::suspension::RR_OCSC());
    

    can_alerts::infos::RearRightSuspension_OCSC_set(io::suspension::RR_OCSC());
    can_alerts::infos::RearLeftSuspension_OCSC_set(io::suspension::RL_OCSC());

    can_tx::RSM_RearLeftSuspensionTravelValid_set(left_travel_Valid);
    can_tx::RSM_RearRightSuspensionTravelValid_set(right_travel_Valid);

    can_tx::RSM_RearLeftSuspensionTravel_set(left_travel_Valid ? left_travel : 0);
    can_tx::RSM_RearRightSuspensionTravel_set(right_travel_Valid ? right_travel : 0);
}
} // namespace app::suspension