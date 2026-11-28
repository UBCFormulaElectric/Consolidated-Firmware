#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

namespace io::imd
{
float getFrequency()
{
    // Frequency set to 0Hz if the signal is DC or not present, as per pwm input driver.
    return imd_pwm_input.get_frequency();
}

float getDutyCycle()
{
    // Duty cycle is returned as a percentage for decoding the IMD status.
    return imd_pwm_input.get_dutyCycle();
}

uint32_t getTimeSincePowerOn()
{
    // IMD and BMS share the same power rail, so uptime = BMS uptime
    return io::time::getCurrentMs();
}
} // namespace io::imd