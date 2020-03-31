#pragma once

#include <assert.h>
#include "auto_generated/App_CanMsgs.h"

struct Imd;

// The IMD encodes a condition in its PWM output's frequency
enum Imd_Condition
{
    IMD_SHORT_CIRCUIT,
    IMD_NORMAL,
    IMD_UNDERVOLTAGE_DETECTED,
    IMD_SST,
    IMD_DEVICE_ERROR,
    IMD_EARTH_FAULT,
    NUM_OF_IMD_CONDITIONS,
    IMD_INVALID = NUM_OF_IMD_CONDITIONS,
};

// Match the IMD enums with the DBC multiplexer values of the IMD message
static_assert(
    IMD_SHORT_CIRCUIT == CANMSGS_IMD_CONDITION_IMD_SHORT_CIRCUIT_CHOICE,
    "The IMD short circuit enum must match its DBC multiplexer value");
static_assert(
    IMD_NORMAL == CANMSGS_IMD_CONDITION_IMD_NORMAL_CHOICE,
    "The IMD normal enum must match its DBC multiplexer value");
static_assert(
    IMD_UNDERVOLTAGE_DETECTED ==
        CANMSGS_IMD_CONDITION_IMD_UNDERVOLTAGE_DETECTED_CHOICE,
    "The IMD undervoltage detected enum must match its DBC multiplexer value");
static_assert(
    IMD_SST == CANMSGS_IMD_CONDITION_IMD_SST_CHOICE,
    "The IMD speed start measurement enum must match its DBC multiplexer "
    "value");
static_assert(
    IMD_DEVICE_ERROR == CANMSGS_IMD_CONDITION_IMD_DEVICE_ERROR_CHOICE,
    "The IMD device error enum must match its DBC multiplexer value");
static_assert(
    IMD_EARTH_FAULT == CANMSGS_IMD_CONDITION_IMD_EARTH_FAULT_CHOICE,
    "The IMD earth fault enum must match its DBC multiplexer value");

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
    struct BmsCanTxInterface *can_tx,
    float (*get_pwm_frequency)(void),
    float (*get_pwm_duty_cycle)(void),
    uint32_t (*get_seconds_since_power_on)(void));

/**
 * Destroy the given IMD object, freeing any memory it uses
 * @param imd
 */
void App_Imd_Destroy(struct Imd *imd);

/**
 * Update the given IMD
 * @param imd The IMD to update
 */
void App_Imd_Tick(struct Imd *const imd);

/**
 * Get the PWM frequency for the given IMD
 * @param imd The IMD to get PWM frequency for
 * @return The PWM frequency for the given IMD
 */
float App_Imd_GetPwmFrequency(const struct Imd *const imd);

/**
 * Get the PWM duty cycle for the given IMD
 * @param imd The IMD to get PWM duty cycle for
 * @return The PWM duty cycle for the given IMD
 */
float App_Imd_GetPwmDutyCycle(const struct Imd *const imd);

/**
 * Get the seconds since power on for the given IMD
 * @param imd The IMD to get seconds since power on for
 * @return The seconds since power on for the given IMD
 */
uint32_t App_Imd_GetSecondsSincePowerOn(const struct Imd *const imd);

/**
 * Get the condition for the given IMD
 * @param imd The IMD to get condition for
 * @return The condition for the given IMD
 */
enum Imd_Condition App_Imd_GetCondition(const struct Imd *const imd);

/**
 * Get the PWM encoding for the given IMD
 * @param imd The IMD to get PWM encoding for
 * @return The PWM encoding for the given IMD
 */
struct Imd_PwmEncoding App_Imd_GetPwmEncoding(const struct Imd *const imd);
