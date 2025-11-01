#include "io_imd.hpp"
#include "io_time.hpp"
#include "hw_pwms.hpp"

constexpr uint8_t PWM_TICKS_MAX = 255;

static uint8_t pwm_counter = 0;

namespace io::imd
{
uint8_t pwmCounterTick() {
    if (pwm_counter != PWM_TICKS_MAX) {
        pwm_counter++;
        return pwm_counter;
    }
    return pwm_counter;
}

float getFrequency() {
    return hw::PwmInput::getFrequency(&hw::pwms::imd_pwm_input);
}

float getDutyCycle() {
    return hw::PwmInput::getDutyCycle(&hw::pwms::imd_pwm_input);
}

uint32_t getTimeSincePowerOn() {
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return io::time::getCurrentMs();
}
} // namespace io::imd