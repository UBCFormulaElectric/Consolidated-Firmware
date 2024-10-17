#pragma once


#include "hw_hal.h"
#include "main.h"

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
class PwmInputs
{

    private:
        TIM_HandleTypeDef *const htim;
        float                    timer_frequency_hz;
        uint32_t                 rising_edge_tim_channel;
        uint32_t                 falling_edge_tim_channel;
        float                    duty_cycle = 0;
        float                    frequency_hz = 0;
    public:
        explicit PwmInputs(TIM_HandleTypeDef *const htim_in, 
                           float timer_frequency_hz_in, 
                           uint32_t rising_edge_tim_channel_in, 
                           uint32_t falling_edge_tim_channel_in) :
                           htim(htim_in), 
                           timer_frequency_hz(timer_frequency_hz_in), 
                           rising_edge_tim_channel(rising_edge_tim_channel_in), 
                           falling_edge_tim_channel(falling_edge_tim_channel_in) {}     
public:

    /**
     * Initialize a PWM input using the given config.
     */
    void init();

    /**
     * Update the frequency and duty cycle for the given PWM input
     */
    void tick();

    /**
     * Get the duty cycle for the given PWM input
     * @return The duty cycle for the given PWM input
     */
    float getDutyCycle();

    /**
     * Get the frequency for the given PWM input
     * @return The frequency for the given PWM input
     */
    float getFrequncy();
};
}