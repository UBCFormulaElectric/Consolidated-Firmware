#include "app_pumpControl.hpp"
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include <app_canTx.hpp>
#include "io_pumps.hpp"
#include <cmath>

namespace app::pumpControl
{
void monitorPumps()
{
    const uint8_t setpoint = can_rx::VC_RLPumpSetpoint_get();
    LOG_IF_ERR(rl_pump.setPercentage(setpoint));
}

void broadcast()
{
    const result<uint8_t> percentage = rl_pump.getPercentage();
    const bool percentage_validity = percentage.has_value() && percentage.value() <= 100;

    can_tx::RSM_RearLeftPumpPercentage_set(percentage.value_or(0));
    can_tx::RSM_RearLeftPumpPercentageValid_set(percentage_validity);
}

} // namespace app::pumpControl