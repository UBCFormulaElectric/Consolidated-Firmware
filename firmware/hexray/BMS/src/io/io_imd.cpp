#include "io_imd.hpp"
#include "io_time.hpp"

constexpr uint8_t PWM_TICKS_MAX = 255;

static uint8_t pwm_counter = 0;

namespace io::imd
{
uint8_t pwmCounterTick(void) {}

float getFrequency(void) {}

float getDutyCycle(void) {}

uint32_t getTimeSincePowerOn(void) {}
} // namespace io::imd