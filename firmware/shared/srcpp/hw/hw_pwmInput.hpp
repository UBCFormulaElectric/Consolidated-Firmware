#pragma once
#include "hw_hal.hpp"
#include "hw_utils.hpp"
#include "util_errorCodes.hpp"
#include "app_timer.hpp"

#include <cstdint>
#include <cassert>

/*
 * PWM input driver notes:
 *
 * 1. The given timer must be initialized with a particular case of input
 *    capture mode:
 *       - Two ICx signals are mapped on the same TIx input.
 *       - These 2 ICx signals are active on edges with opposite polarity.
 *       - One of the two TIxFP signals is selected as trigger input and the
 *         slave mode controller is configured in reset mode or not in
 *         reset mode (Adjust the class paramters accordingly).
 * 2. Not all timers support this particular case of input capture mode.
 *       Consult the datasheet to double check!
 *
 * PWM input driver configuration parameters:
 *
 * htim: The handle of the timer measuring the PWM input
 * timer_frequency_hz: The frequency of the timer measuring the PWM input
 * rising_edge_tim_channel: The rising edge channel of the timer measuring the
 * PWM input
 * falling_edge_tim_channel: The falling edge channel of the timer measuring
 * the PWM input
 */

constexpr uint32_t MIN_ACTIVE_TIMER_DURATION_MS = 1U;
constexpr uint32_t MAX_ACTIVE_TIMER_DURATION_MS = 10000U;

namespace hw
{

// Descriptive undefined functions to trigger readable compile-time errors in consteval contexts
void ERROR_PWM_TIMER_FREQUENCY_TOO_LOW_FOR_RESOLUTION();

class PwmInput
{
  private:
    enum class PwmMode
    {
        PWMINPUT,
        PWMFREQONLY,
    };

    /* Conifguration for PWM input*/
    TIM_HandleTypeDef    &htim;
    HAL_TIM_ActiveChannel tim_active_channel;
    float                 tim_frequency_hz;
    uint32_t              rising_edge_tim_channel;
    uint32_t              falling_edge_tim_channel;
    uint32_t              tim_auto_reload_reg;
    bool                  reset_mode;

    mutable bool pwm_active = true;
    mutable bool first_tick = false;

    /* Calculaing frequency based of rising edge*/
    mutable uint32_t curr_rising_edge = 0;
    mutable uint32_t prev_rising_edge = 0;
    mutable uint32_t falling_edge     = 0;

    /*Outputs of PWM input*/
    mutable float duty_cycle   = 0;
    mutable float frequency_hz = 0;

    PwmMode            mode;
    mutable app::Timer active_timer;

    [[nodiscard]] static consteval uint32_t clampActiveTimerDuration(const uint32_t duration_ms)
    {
        if (duration_ms < MIN_ACTIVE_TIMER_DURATION_MS)
        {
            return MIN_ACTIVE_TIMER_DURATION_MS;
        }

        if (duration_ms > MAX_ACTIVE_TIMER_DURATION_MS)
        {
            return MAX_ACTIVE_TIMER_DURATION_MS;
        }

        return duration_ms;
    }

    [[nodiscard]] static consteval uint32_t getActiveTimerDurationFromFrequency(const float min_expected_frequency_hz)
    {
        return clampActiveTimerDuration(static_cast<uint32_t>((1.0f / min_expected_frequency_hz) * 2.0 * 1000.0f));
    }

    /**
     * Set the frequency for the given PWM input
     * @param curr_frequency Frequency, in Hz
     */
    void setFrequency(const float curr_frequency) const
    {
        assert(frequency_hz >= 0.0f);
        frequency_hz = curr_frequency;
    };

    /**
     * Set the duty cycle for the given PWM input
     * @param curr_duty_cycle Duty cycle, in percentage (0-100)
     */
    void setDutyCycle(const float curr_duty_cycle) const
    {
        assert(curr_duty_cycle >= 0.0f);
        if (curr_duty_cycle > 100.0f)
        {
            // Since continuing from a breakpoint can mess with the captured values,
            // we want to avoid setting an invalid duty cycle in that case.
            // Instead, we just ignore the new captured value and keep the previous duty cycle.
            return;
        }
        duty_cycle = curr_duty_cycle;
    };

  public:
    /* Complete PWM Input Constructor*/
    consteval explicit PwmInput(
        TIM_HandleTypeDef          &htim_in,
        const HAL_TIM_ActiveChannel tim_active_channel_in,
        const float                 tim_frequency_hz_in,
        const uint32_t              rising_edge_tim_channel_in,
        const uint32_t              falling_edge_tim_channel_in,
        const uint32_t              tim_auto_reload_reg_in,
        const bool                  reset_mode_in,
        const uint32_t              pwm_min_frequency_hz_in)
      : htim(htim_in),
        tim_active_channel(tim_active_channel_in),
        tim_frequency_hz(tim_frequency_hz_in),
        rising_edge_tim_channel(rising_edge_tim_channel_in),
        falling_edge_tim_channel(falling_edge_tim_channel_in),
        tim_auto_reload_reg(tim_auto_reload_reg_in),
        reset_mode(reset_mode_in),
        mode(PwmMode::PWMINPUT),
        active_timer(getActiveTimerDurationFromFrequency(pwm_min_frequency_hz_in))
    {
        // 1. Assert PWM isn't too fast (Ensure we get enough ticks for resolution)
        // e.g. We want AT LEAST 100 ticks per PWM period to measure it passably
        if ((tim_frequency_hz_in / static_cast<float>(pwm_min_frequency_hz_in)) < 100.0f)
        {
            ERROR_PWM_TIMER_FREQUENCY_TOO_LOW_FOR_RESOLUTION();
        }
    }

    /* Frequency only PWM Input Constructor */
    consteval explicit PwmInput(
        TIM_HandleTypeDef          &htim_in,
        const HAL_TIM_ActiveChannel tim_active_channel_in,
        const float                 tim_frequency_hz_in,
        const uint32_t              rising_edge_tim_channel_in,
        const uint32_t              tim_auto_reload_reg_in,
        const bool                  reset_mode_in,
        const float                 pwm_min_frequency_hz_in)
      : htim(htim_in),
        tim_active_channel(tim_active_channel_in),
        tim_frequency_hz(tim_frequency_hz_in),
        rising_edge_tim_channel(rising_edge_tim_channel_in),
        falling_edge_tim_channel(0),
        tim_auto_reload_reg(tim_auto_reload_reg_in),
        reset_mode(reset_mode_in),
        mode(PwmMode::PWMFREQONLY),
        active_timer(getActiveTimerDurationFromFrequency(pwm_min_frequency_hz_in))
    {
        // 1. Assert PWM isn't too fast (Ensure we get enough ticks for resolution)
        // e.g. We want AT LEAST 100 ticks per PWM period to measure it passably
        if ((tim_frequency_hz_in / static_cast<float>(pwm_min_frequency_hz_in)) < 100.0f)
        {
            ERROR_PWM_TIMER_FREQUENCY_TOO_LOW_FOR_RESOLUTION();
        }
    }

    PwmInput() = delete;

    /**
     * Initialize a frequency-only PWM input using the given (hardware) timer
     *
     * @note The given timer must be initialized with Input Capture Direct Mode
     */
    [[nodiscard]] std::expected<void, ErrorCode> init() const
    {
        RETURN_IF_ERR_SILENT(hw::utils::convertHalStatus(HAL_TIM_IC_Start_IT(&htim, rising_edge_tim_channel)));
        RETURN_IF_ERR_SILENT(hw::utils::convertHalStatus(HAL_TIM_IC_Start(&htim, falling_edge_tim_channel)));
        RETURN_IF_ERR_SILENT(hw::utils::convertHalStatus(HAL_TIM_Base_Start_IT(&htim)));
        this->active_timer.restart();
        return {};
    }

    /**
     * Get the frequency for the given PWM input
     * @return The frequency for the given PWM input
     */
    [[nodiscard]] float get_frequency() const { return this->pwm_active ? frequency_hz : 0.0f; }

    /**
     * Get the duty cycle for the given PWM input
     * @return The duty cycle for the given PWM input
     */
    [[nodiscard]] float get_dutyCycle() const { return this->pwm_active ? duty_cycle : 0.0f; }

    /**
     * Get the timer handle for the given PWM input
     * @return The timer handle for the given PWM input
     */
    [[nodiscard]] constexpr TIM_HandleTypeDef &get_timer_handle() const { return htim; }

    [[nodiscard]] constexpr HAL_TIM_ActiveChannel get_timer_activeChannel() const { return tim_active_channel; }
    /**
     * Check if the given PWM signal is active. If the sensor detects a DC signal
     * set the frequency to 0Hz.
     * @note This function should be called in the timer overflow interrupt
     *       for the PWM signal
     */
    [[nodiscard]] bool pwm_isActive() const
    {
        // If for two periods of the minimum expected frequency,
        // we haven't detected a rising edge, we can conclude that the PWM signal is inactive
        this->pwm_active = (this->active_timer.updateAndGetState()) != app::Timer::TimerState::EXPIRED;
        return this->pwm_active;
    }

    /**
     * Update the frequency and duty cycle for the given PWM input
     */
    void tick() const
    {
        this->active_timer.restart();

        // We store the counter values captured during two most recent rising edges.
        // The difference between these two counter values is used to compute the
        // frequency of the PWM input signal.
        uint32_t rising_edge_delta = 0;

        if (first_tick)
        {
            curr_rising_edge = HAL_TIM_ReadCapturedValue(&htim, rising_edge_tim_channel);
            first_tick       = false;
        }
        else
        {
            prev_rising_edge = curr_rising_edge;

            curr_rising_edge = HAL_TIM_ReadCapturedValue(&htim, rising_edge_tim_channel);

            if (mode == PwmMode::PWMINPUT)
            {
                falling_edge = HAL_TIM_ReadCapturedValue(&htim, falling_edge_tim_channel);
            }

            if (reset_mode)
            {
                // In reset mode, the timer counter is reset on every rising edge, captured value gives the period.
                rising_edge_delta = curr_rising_edge;
            }
            else if (curr_rising_edge > prev_rising_edge)
            {
                rising_edge_delta = curr_rising_edge - prev_rising_edge;
            }
            else
            {
                // Occurs when the counter rolls over
                rising_edge_delta = tim_auto_reload_reg - prev_rising_edge + curr_rising_edge;
            }

            if (rising_edge_delta != 0)
            {
                setFrequency(tim_frequency_hz / static_cast<float>(rising_edge_delta));
            }
            else
            {
                // This should never occur for non-reset modes, if curr_rising_edge == prev_rising_edge,
                // we assume that the the timer frequency is exactly equal to the PWM frequency.
                // The asserts in the constructor should ensure that this doesn't cause issues with resolution.
                setFrequency(0.0f);
            }

            if (mode == PwmMode::PWMINPUT)
            {
                // Calculating duty cycle
                if (rising_edge_delta != 0)
                {
                    uint32_t high_time;
                    // Since we only trigger the interrupt for the tick function on rising edge,
                    // it means our captured falling edge is for the previous period.
                    if (reset_mode)
                    {
                        high_time = falling_edge;
                    }
                    else if (falling_edge < prev_rising_edge)
                    {
                        high_time = falling_edge + (tim_auto_reload_reg - prev_rising_edge);
                    }
                    else
                    {
                        high_time = falling_edge - prev_rising_edge;
                    }
                    setDutyCycle(100.0f * static_cast<float>(high_time) / static_cast<float>(rising_edge_delta));
                }
            }
        }
    }
};
} // namespace hw
