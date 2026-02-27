#pragma once
#include "hw_hal.hpp"
#include "hw_utils.hpp"
#include "util_errorCodes.hpp"

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
 *         slave mode controller is configured in reset mode.
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

namespace hw
{

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

    mutable bool first_tick = false;

    /* Calculaing frequency based of rising edge*/
    mutable uint32_t curr_rising_edge = 0;
    mutable uint32_t prev_rising_edge = 0;

    /*Outputs of PWM input*/
    mutable float  duty_cycle         = 0;
    mutable float  frequency_hz       = 0;
    mutable size_t tim_overflow_count = 0;

    PwmMode mode;

    /**
     * Set the frequency for the given PWM input
     * @param curr_frequency Frequency, in Hz
     */
    void setFrequency(const float curr_frequency) const
    {
        assert(frequency_hz >= 0.0f);
        frequency_hz = curr_frequency;
    };

  public:
    /* Complete PWM Input Constructor*/
    consteval explicit PwmInput(
        TIM_HandleTypeDef          &htim_in,
        const HAL_TIM_ActiveChannel tim_active_channel_in,
        const float                 tim_frequency_hz_in,
        const uint32_t              rising_edge_tim_channel_in,
        const uint32_t              falling_edge_tim_channel_in,
        const uint32_t              tim_auto_reload_reg_in)
      : htim(htim_in),
        tim_active_channel(tim_active_channel_in),
        tim_frequency_hz(tim_frequency_hz_in),
        rising_edge_tim_channel(rising_edge_tim_channel_in),
        falling_edge_tim_channel(falling_edge_tim_channel_in),
        tim_auto_reload_reg(tim_auto_reload_reg_in),
        mode(PwmMode::PWMINPUT)
    {
    }

    /* Frequency only PWM Input Constructor */
    consteval explicit PwmInput(
        TIM_HandleTypeDef          &htim_in,
        const HAL_TIM_ActiveChannel tim_active_channel_in,
        const float                 tim_frequency_hz_in,
        const uint32_t              rising_edge_tim_channel_in,
        const uint32_t              tim_auto_reload_reg_in)
      : htim(htim_in),
        tim_active_channel(tim_active_channel_in),
        tim_frequency_hz(tim_frequency_hz_in),
        rising_edge_tim_channel(rising_edge_tim_channel_in),
        falling_edge_tim_channel(0),
        tim_auto_reload_reg(tim_auto_reload_reg_in),
        mode(PwmMode::PWMFREQONLY)
    {
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
        RETURN_IF_ERR_SILENT(hw::utils::convertHalStatus(HAL_TIM_IC_Start_IT(&htim, falling_edge_tim_channel)));
        RETURN_IF_ERR_SILENT(hw::utils::convertHalStatus(HAL_TIM_Base_Start_IT(&htim)));
        return {};
    }

    /**
     * Get the frequency for the given PWM input
     * @return The frequency for the given PWM input
     */
    [[nodiscard]] float get_frequency() const { return frequency_hz; }

    /**
     * Get the duty cycle for the given PWM input
     * @return The duty cycle for the given PWM input
     */
    [[nodiscard]] float get_dutyCycle() const { return duty_cycle; }

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
    [[nodiscard]] std::expected<void, ErrorCode> pwm_isActive()
    {
        // If the timer overflows twice without a rising edge, the PWM signal is
        // likely inactive (i.e. DC signal) and its frequency can't be computed
        if (++tim_overflow_count == 2U)
        {
            setFrequency(0);
            return std::unexpected(ErrorCode::ERROR);
        }
        return {};
    }

    /**
     * Update the frequency and duty cycle for the given PWM input
     */
    void tick() const
    {
        // Reset the timer overflow count to indicate that the PWM signal is active
        tim_overflow_count = 0;

        // We store the counter values captured during two most recent rising edges.
        // The difference between these two counter values is used to compute the
        // frequency of the PWM input signal.
        if (first_tick)
        {
            curr_rising_edge = HAL_TIM_ReadCapturedValue(&htim, rising_edge_tim_channel);
            first_tick       = false;
        }
        else
        {
            prev_rising_edge = curr_rising_edge;

            curr_rising_edge = HAL_TIM_ReadCapturedValue(&htim, rising_edge_tim_channel);

            uint32_t rising_edge_detlta;

            if (curr_rising_edge > prev_rising_edge)
            {
                rising_edge_detlta = curr_rising_edge - prev_rising_edge;
                setFrequency(tim_frequency_hz / static_cast<float>(rising_edge_detlta));
            }
            else if (curr_rising_edge < prev_rising_edge)
            {
                // Occurs when the counter rolls over
                rising_edge_detlta = tim_auto_reload_reg - prev_rising_edge + curr_rising_edge;
                setFrequency(tim_frequency_hz / static_cast<float>(rising_edge_detlta));
            }
            else
            {
                // Occurs when the counter rolls over (i.e. The PWM frequency being
                // measured is too low), or either when a tick arrives before the
                // counter can upcount (i.e. The PWM frequency being measured is
                // too high)
                setFrequency(0);
            }
        }

        if (mode == PwmMode::PWMINPUT)
        {
            // Calculating duty cycle
            if (curr_rising_edge != 0)
            {
                const uint32_t falling_edge = HAL_TIM_ReadCapturedValue(&htim, falling_edge_tim_channel);

                duty_cycle = 100.0f * static_cast<float>(falling_edge) / static_cast<float>(curr_rising_edge);
            }
        }
    }
};
} // namespace hw