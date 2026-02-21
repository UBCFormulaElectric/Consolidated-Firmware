#include "app_suspension.hpp"

#include "io_suspension.hpp"
#include "app_canTx.hpp"

namespace app::suspension
{
void broadcast()
{
    const float left_travel  = io::suspension::getLeftTravel();
    const float right_travel = io::suspension::getRightTravel();
    const bool  left_ocsc    = io::suspension::leftSensorOCSC();
    const bool  right_ocsc   = io::suspension::rightSensorOCSC();
    can_tx::FSM_LeftSuspensionTravel_set(left_travel);
    can_tx::FSM_RightSuspensionTravel_set(right_travel);
    can_tx::FSM_Info_LeftSuspensionOCSC_set(left_ocsc);
    can_tx::FSM_Info_RightSuspensionOCSC_set(right_ocsc);
}
} // namespace app::suspension
