#include "hw_pwmOutput.hpp"

#include "hw_utils.hpp"

std::expected<void, ErrorCode> hw::PwmOutput::start() const
{
    return hw_utils_convertHalStatus(HAL_TIM_PWM_Start(htim, pwm_channel));
}

std::expected<void, ErrorCode> hw::PwmOutput::stop() const
{
    return hw_utils_convertHalStatus(HAL_TIM_PWM_Stop(htim, pwm_channel));
}

std::expected<void, ErrorCode> hw::PwmOutput::setDutyCycle(const float duty_cycle_in) const
{
    // Constrain duty cycle between 0.0% and 100.0%.
    if (duty_cycle_in < 0.0f || duty_cycle_in > 100.0f)
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }
    duty_cycle = duty_cycle_in;

    // Get current ARR value
    const uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
    // Calculate CCR value
    const auto ccr_value = static_cast<uint32_t>((duty_cycle / 100.0f) * (static_cast<float>(arr + 1)));

    // Set duty cycle
    // TODO what to do with x?
    const auto x = __HAL_TIM_SET_COMPARE(htim, pwm_channel, ccr_value);
    return {};
}

void hw::PwmOutput::getFrequency(const float frequency_hz_in) // TODO wtf are we doing here
{
    frequency_hz = frequency_hz_in;
}