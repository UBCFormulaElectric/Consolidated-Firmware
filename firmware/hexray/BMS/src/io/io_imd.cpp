#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

namespace io::imd
{
<<<<<<< Updated upstream
uint8_t pwmCounterTick()
{
    if (pwm_counter != PWM_TICKS_MAX)
    {
        pwm_counter++;
        return pwm_counter;
    }
    return pwm_counter;
}

float getFrequency()
{
    return hw::PwmInput::getFrequency(&hw::pwms::imd_pwm_input);
=======
float getFrequency() {
    if (!hw::pwms::imd_pwm_input.pwm_isActive()) {
        return 0.0f;
    }
    
    return hw::pwms::imd_pwm_input.get_frequency();
>>>>>>> Stashed changes
}

float getDutyCycle()
{
    return hw::PwmInput::getDutyCycle(&hw::pwms::imd_pwm_input);
}

<<<<<<< Updated upstream
uint32_t getTimeSincePowerOn()
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
=======
uint32_t getTimeSincePowerOn() {
    // IMD and BMS share the same power rail, so uptime = BMS uptime
>>>>>>> Stashed changes
    return io::time::getCurrentMs();
}
} // namespace io::imd