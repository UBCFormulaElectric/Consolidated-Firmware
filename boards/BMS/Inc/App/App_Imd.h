#pragma once

struct Imd;

#include "auto_generated/App_CanTx.h"

/**
 * Allocate and initialize an IMD
 * @get_pwm_frequency: A function that can be called to get the frequency of the
 *                     IMD's PWM output
 * @get_pwm_duty_cycle: A function that can be called to get the duty cycle of
 *                      the IMD's PWM output
 * @get_seconds_since_power_on: A function that can be called to get the time of
 *                              the system, in milliseconds
 * @return A pointer to the created IMD, whose ownership is given to the caller
 */
struct Imd *App_Imd_Create(
    struct BMSCanTxInterface *can_tx,
    float (*get_pwm_frequency)(void),
    float (*get_pwm_duty_cycle)(void),
    uint32_t (*get_seconds_since_power_on)(void));

/**
 * Destroys the given IMD object, freeing any memory it uses
 * @param imd
 */
void App_Imd_Destroy(struct Imd *imd);

/**
 * Update the given IMD
 * @param imd: The IMD to update
 */
void App_Imd_Tick(struct Imd *const imd);
