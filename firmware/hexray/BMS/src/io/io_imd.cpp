#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

namespace io::imd
{
float getFrequency()
{
    if (!hw::PwmInput::pwm_isActive(&imd_pwm_input))
    {
        return 0.0f;
    }

    return hw::PwmInput::getFrequency(&imd_pwm_input);
}

float getDutyCycle()
{
    return hw::PwmInput::getDutyCycle(&imd_pwm_input);
}

uint32_t getTimeSincePowerOn()
{
    // IMD and BMS share the same power rail, so uptime = BMS uptime
    return io::time::getCurrentMs();
}
} // namespace io::imd