#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

using namespace hw::pwm;

namespace io::imd
{
float getFrequency()
{
    // Frequency set to 0Hz if the signal is DC or not present, as per pwm input driver.
    imd_pwm_input.pwm_isActive();

    return imd_pwm_input.get_frequency();
}

float getDutyCycle()
{
    return imd_pwm_input.get_dutyCycle();
}

uint32_t getTimeSincePowerOn()
{
    // IMD and BMS share the same power rail, so uptime = BMS uptime
    return io::time::getCurrentMs();
}
} // namespace io::imd
