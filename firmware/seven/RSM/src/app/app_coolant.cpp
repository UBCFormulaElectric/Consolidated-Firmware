#include "app_coolant.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "app_rangeCheck.hpp"
#include "app_signal.hpp"

#include "io_coolant.hpp"

static constexpr float MIN_FLOW_RATE_L_PER_MIN = 1.0f;
static constexpr float MAX_FLOW_RATE_L_PER_MIN = 30.0f;

static constexpr uint32_t FLOW_METER_TIME_TO_FAULT = 1000U;
static constexpr uint32_t FLOW_METER_TIME_TO_CLEAR = 1000U;

namespace app::coolant
{
static Signal               flow_in_range_signal(FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);
static constexpr RangeCheck flow_rate_in_range_check = RangeCheck(MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN);

void broadcast()
{
    const float flow_val = io::coolant::getFlowRate();
    can_tx::RSM_CoolantFlowRate_set(flow_rate_in_range_check.get_clamped_value(flow_val));
    can_alerts::infos::FlowRateOutOfRange_set(
        flow_rate_in_range_check.get_status(flow_val) != RangeCheck::Status::VALUE_IN_RANGE);
}
} // namespace app::coolant