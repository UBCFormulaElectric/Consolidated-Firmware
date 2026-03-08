#include "app_faultHandling.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"

namespace app::fault_handling
{
constexpr uint8_t CONTACTOR_STATE_CLOSED = 1u;

bool air_minus_closed(void)
{
    return (CONTACTOR_STATE_CLOSED == app::canRx::BMS_IrNegative_get());
}

bool fsm_bms_HeartbeartChecks(void)
{
    // VC heartbeat check
    //
    return app::can_alerts::faults::BoardHasFault(app::canutils::CanNode::VC_NODE);
}
} // namespace app::fault_handling