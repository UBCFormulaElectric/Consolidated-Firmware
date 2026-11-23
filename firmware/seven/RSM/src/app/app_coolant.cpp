#include "app_coolant.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "app_rangeCheck.hpp"
#include "app_signal.hpp"

#include "io_coolant.hpp"

static constexpr float MIN_FLOW_RATE_L_PER_MIN = 1.0f;
static constexpr float MAX_FLOW_RATE_L_PER_MIN = 30.0f;

// Todo: Test and implement appropriate debounce timing during integration
static constexpr uint32_t FLOW_METER_TIME_TO_FAULT = 50U;
static constexpr uint32_t FLOW_METER_TIME_TO_CLEAR = 50U;

namespace app::coolant
{
static Signal               flow_in_range_signal(FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);
static constexpr RangeCheck flow_rate_in_range_check = RangeCheck(MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN);

void broadcast()
{
    const float flow_val = io::coolant::getFlowRate();
    can_tx::RSM_CoolantFlowRate_set(flow_rate_in_range_check.get_clamped_value(flow_val));

    const bool in_range = (flow_rate_in_range_check.get_status(flow_val) == RangeCheck::Status::VALUE_IN_RANGE);

    const Signal::SignalState state = flow_in_range_signal.get_updated_state(not(in_range), in_range);

    can_alerts::infos::FlowRateOutOfRange_set(state == Signal::SignalState::ACTIVE);
}
} // namespace app::coolant