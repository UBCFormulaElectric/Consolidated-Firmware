#include "io_coolant.hpp"
#include "hw_pwms.hpp"


constexpr float FREQ_TO_LITERS_PER_MINUTE = 7.5f;

namespace io::coolant
{
void init()
{
    hw::pwm::flow_meter_config.init();
}

float getFlowRate()
{
    // const float freq_read = hw_pwmInputFreqOnly_getFrequency(&flow_meter_5v5);
    // return freq_read * FREQ_TO_LITERS_PER_MINUTE;
    float frequency = hw::pwm::flow_meter_config.get_frequency();
    return frequency * FREQ_TO_LITERS_PER_MINUTE;
}

void checkIfFlowMeterActive()
{
    // hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter_5v5);
}
} // namespace io::coolant

// namespace io::coolant
