#pragma once

#include "app_utils.hpp"
#include <cstdint>
#include <array>
#include "hw_hal.hpp"
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
    TIM_HandleTypeDef    *htim = NULL;
    HAL_TIM_ActiveChannel tim_active_channel;
    float                 tim_frequency_hz;
    uint32_t              rising_edge_tim_channel;
    uint32_t              falling_edge_tim_channel;
    uint32_t              tim_auto_reload_reg;

    bool first_tick = false;

    /* Calculaing frequency based of rising edge*/
    uint32_t curr_rising_edge = 0;
    uint32_t prev_rising_edge = 0;

    /*Outputs of PWM input*/
    float  duty_cycle         = 0;
    float  frequency_hz       = 0;
    size_t tim_overflow_count = 0;

    PwmMode mode;


    /**
     * Set the frequency for the given PWM input
     * @param frequency_hz: Frequency, in Hz
     */
    void setFrequency(const float curr_frequency)
    {
        assert(frequency_hz >= 0.0f);
        frequency_hz = curr_frequency;
    };

  public:

    /* Complete PWM Input Constructor*/
    consteval explicit PwmInput(
        TIM_HandleTypeDef    *htim,
        HAL_TIM_ActiveChannel tim_active_channel,
        float                 tim_frequency_hz,
        uint32_t              rising_edge_tim_channel,
        uint32_t              falling_edge_tim_channel,
        uint32_t              tim_auto_reload_reg)
      : htim(htim),
        tim_active_channel(tim_active_channel),
        tim_frequency_hz(tim_frequency_hz),
        rising_edge_tim_channel(rising_edge_tim_channel),
        falling_edge_tim_channel(falling_edge_tim_channel),
        tim_auto_reload_reg(tim_auto_reload_reg),
        mode(PwmMode::PWMINPUT) {}

    /* Frequency only PWM Input Constructor */
    consteval explicit PwmInput(
        TIM_HandleTypeDef    *htim,
        HAL_TIM_ActiveChannel tim_active_channel,
        float                 tim_frequency_hz,
        uint32_t              rising_edge_tim_channel,
        uint32_t              tim_auto_reload_reg)
      : htim(htim),
        tim_active_channel(tim_active_channel),
        tim_frequency_hz(tim_frequency_hz),
        rising_edge_tim_channel(rising_edge_tim_channel),
        falling_edge_tim_channel(0),
        tim_auto_reload_reg(tim_auto_reload_reg),
        mode(PwmMode::PWMFREQONLY) {}

    /**
     * Initialize a frequency-only PWM input using the given (hardware) timer
     *
     * @note The given timer must be initialized with Input Capture Direct Mode
     */
    void init();

    /**
     * Get the frequency for the given PWM input
     * @return The frequency for the given PWM input
     */
    float get_frequency(void) const;

    /**
     * Get the duty cycle for the given PWM input
     * @return The duty cycle for the given PWM input
     */
    float get_dutyCycle(void) const;

    /**
     * Get the timer handle for the given PWM input
     * @return The timer handle for the given PWM input
     */
    constexpr TIM_HandleTypeDef *get_timer_handle(void) const
    {
        return htim;
    }

    constexpr HAL_TIM_ActiveChannel get_timer_activeChannel(void) const
    {
        return tim_active_channel;
    }
    /**
     * Check if the given PWM signal is active. If the sensor detects a DC signal
     * set the frequency to 0Hz.
     * @note This function should be called in the timer overflow interrupt
     *       for the PWM signal
     */
    bool pwm_isActive(void);

    /**
     * Update the frequency and duty cycle for the given PWM input
     */
    void tick(void);
};
} // namespace hw