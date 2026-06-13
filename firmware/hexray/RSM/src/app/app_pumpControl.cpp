#include "app_pumpControl.hpp"
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include <app_canTx.hpp>
#include "io_pumps.hpp"

namespace app::pumpControl
{
void monitorPumps()
{
    const uint8_t setpoint = can_rx::VC_RLPumpSetpoint_get();
    LOG_IF_ERR(rl_pump.setPercentage(setpoint));
}

void broadcast()
{
    if (const result<uint8_t> percentage = rl_pump.getPercentage())
    {
        can_tx::RSM_RearLeftPumpPercentage_set(percentage.value());
    }
    else
    {
        can_tx::RSM_RearLeftPumpPercentage_set(255);
    }
}

} // namespace app::pumpControl