#include "app_coolant.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
extern "C"
{
#include "app_rangeCheck.h"
}
#include "app_signal.hpp"

#include "io_coolant.hpp"

static constexpr float MIN_FLOW_RATE_L_PER_MIN = 1.0f;
static constexpr float MAX_FLOW_RATE_L_PER_MIN = 30.0f;

static constexpr uint32_t   FLOW_METER_TIME_TO_FAULT = 1000U;
static constexpr uint32_t   FLOW_METER_TIME_TO_CLEAR = 1000U;
static constexpr RangeCheck flow_rate_in_range_check = { nullptr, MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN };

namespace app::coolant
{
static Signal flow_in_range_signal(FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);

void broadcast()
{
    const float flow_val               = io::coolant::getFlowRate();
    const auto [status, clamped_value] = app_rangeCheck_getValue(&flow_rate_in_range_check, flow_val);
    can_tx::RSM_CoolantFlowRate_set(flow_val);
    can_alerts::infos::FlowRateOutOfRange_set(status != VALUE_IN_RANGE);
}
} // namespace app::coolant