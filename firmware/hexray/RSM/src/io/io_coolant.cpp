#include "io_coolant.hpp"

constexpr float FREQ_TO_LITERS_PER_MINUTE = 7.5f;

namespace io::coolant
{
void init()
{
    LOG_IF_ERR(hw::pwm::flow_meter_config.init());
}

float getFlowRate()
{
    float frequency = hw::pwm::flow_meter_config.get_frequency();
    return frequency * FREQ_TO_LITERS_PER_MINUTE;
}

std::expected<void, ErrorCode> checkIfFlowMeterActive()
{
    return hw::pwm::flow_meter_config.pwm_isActive();
}
} // namespace io::coolant
