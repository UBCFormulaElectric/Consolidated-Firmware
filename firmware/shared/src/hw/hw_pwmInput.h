#pragma once

#include "hw_hal.h"

#define PWM_TICKS_MAX 255

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
typedef struct
{
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
    uint32_t           falling_edge_tim_channel;
} PwmInputConfig;

typedef struct
{
    float                 duty_cycle;
    float                 frequency_hz;
    const PwmInputConfig *config;
} PwmInput;

/**
 * Initialize a PWM input using the given config.
 * @param pwm_input PWM input instance.
 * @param config Configuration struct.
 */
void io_pwmInput_init(PwmInput *pwm_input, const PwmInputConfig *config);

/**
 * Update the frequency and duty cycle for the given PWM input
 * @param pwm_input: The PWM input to update for
 */
void hw_pwmInput_tick(PwmInput *pwm_input);

/**
 * Get the duty cycle for the given PWM input
 * @param pwm_input: The PWM input to get duty cycle for
 * @return The duty cycle for the given PWM input
 */
float hw_pwmInput_getDutyCycle(const PwmInput *pwm_input);

/**
 * Get the frequency for the given PWM input
 * @param pwm_input: The PWM input to get frequency for
 * @return The frequency for the given PWM input
 */
float hw_pwmInput_getFrequency(const PwmInput *pwm_input);

/**
 * Increment the number of ticks since the last pwm was interpreted from the imd.
 * @return The number of ticks since the last pwm was interpreted from the imd.
 */
uint8_t hw_pwm_counter_tick(void);
