#include "io_coolant.hpp"
#include "hw_pwms.hpp"

constexpr float FREQ_TO_LITERS_PER_MINUTE = 0.147f;
constexpr float FREQ_OFFSET               = -0.4711f;

namespace io::coolant
{
void init()
{
    LOG_IF_ERR(flow_meter_config.init());
}

float getFlowRate()
{
    const float frequency = flow_meter_config.get_frequency();
    return frequency * FREQ_TO_LITERS_PER_MINUTE + FREQ_OFFSET;
}

bool checkIfFlowMeterActive()
{
    return flow_meter_config.pwm_isActive();
}
} // namespace io::coolant
