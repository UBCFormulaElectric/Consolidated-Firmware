#include "app_faultHandling.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"

namespace app::fault_handling
{

bool air_minus_closed(void)
{
    return (app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED == app::can_rx::BMS_IrNegative_get());
}

bool HeartbeartChecks(void)
{
    // VC heartbeat check
    return app::can_alerts::BoardHasFault(app::can_utils::CanNode::VC_NODE);
}
} // namespace app::fault_handling