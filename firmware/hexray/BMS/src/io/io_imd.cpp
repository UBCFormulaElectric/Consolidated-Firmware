#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

namespace io::imd
{
float getFrequency()
{
    if (!hw::pwms::imd_pwm_input.pwm_isActive())
    {
        return 0.0f;
    }

    return hw::pwms::imd_pwm_input.get_frequency();
}

float getDutyCycle()
{
    return hw::pwms::imd_pwm_input.get_dutyCycle();
}

uint32_t getTimeSincePowerOn()
{
    // IMD and BMS share the same power rail, so uptime = BMS uptime
    return io::time::getCurrentMs();
}
} // namespace io::imd
