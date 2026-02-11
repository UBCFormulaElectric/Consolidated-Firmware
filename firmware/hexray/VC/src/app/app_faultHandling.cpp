#include "app_faultHandling.hpp"
extern "C"
{
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_canAlerts.h"
#include "app_stateMachine.h"
#include "states/app_states.h"
}

namespace app::fault_handling
{
constexpr uint8_t CONTACTOR_STATE_CLOSED = 1u;

bool air_minus_closed(void)
{;
    return (CONTACTOR_STATE_CLOSED == app::canRx::BMS_IrNegative_get());
}

bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
{
    return app_canAlerts_BoardHasFault(VC_NODE);
}
} // namespace app::fault_handling