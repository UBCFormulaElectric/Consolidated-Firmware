#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Imd;

// States for speed start measurement
enum SST { SST_GOOD, SST_BAD, SST_INVALID };

enum Imd_ConditionName {
  IMD_SHORT_CIRCUIT,
  IMD_NORMAL,
  IMD_UNDERVOLTAGE_DETECTED,
  IMD_SST,
  IMD_DEVICE_ERROR,
  IMD_GROUND_FAULT,
  NUM_OF_IMD_CONDITIONS,
  IMD_INVALID = NUM_OF_IMD_CONDITIONS,
};

// The IMD has an PWM output that encodes information about the IMD condition:
//   - The frequency encodes a name
//   - The duty cycle encodes additional information (where applicable)
struct Imd_Condition {
  enum Imd_ConditionName name;
  struct {
    bool valid_duty_cycle;
    union {
      // 10 and 20Hz: Insulation measurement DCP
      uint16_t insulation_measurement_dcp_kohms;
      // 30Hz: Speed Start Measurement
      enum SST speed_start_status;
      // 0Hz, 40Hz, 50Hz: PWM doesn't encode any information
    };
  } pwm_encoding;
};

/**
 * Allocate and initialize an IMD
 * @get_pwm_frequency: A function that can be called to get the frequency of the
 *                     IMD's PWM output
 * @pwm_frequency_tolerance: The acceptable tolerance when we map the IMD's PWM
 *                           output frequency to a condition
 * @get_pwm_duty_cycle: A function that can be called to get the duty cycle of
 *                      the IMD's PWM output
 * @get_seconds_since_power_on: A function that can be called to get the time of
 *                              the system, in milliseconds
 * @return A pointer to the created IMD, whose ownership is given to the caller
 */
struct Imd *App_Imd_Create(float (*get_pwm_frequency)(void),
                           float pwm_frequency_tolerance,
                           float (*get_pwm_duty_cycle)(void),
                           uint16_t (*get_seconds_since_power_on)(void));

/**
 * Deallocate the memory used by the given IMD
 * @param imd The IMD to deallocate
 */
void App_Imd_Destroy(struct Imd *imd);

/**
 * Get the condition for the given IMD
 * @param imd The IMD to get condition for
 * @return The condition for the given IMD
 */
struct Imd_Condition App_Imd_GetCondition(const struct Imd *imd);

/**
 * Get the PWM frequency for the given IMD
 * @param imd The IMD to get PWM frequency for
 * @return The PWM frequency for the given IMD
 */
float App_Imd_GetPwmFrequency(const struct Imd *imd);

/**
 * Get the PWM duty cycle for the given IMD
 * @param imd The IMD to get PWM duty cycle for
 * @return The PWM duty cycle for the given IMD
 */
float App_Imd_GetPwmDutyCycle(const struct Imd *imd);

/**
 * Get the seconds since power on for the given IMD
 * @param imd The IMD to get seconds since power on for
 * @return The seconds since power on for the given IMD
 */
uint16_t App_Imd_GetSecondsSincePowerOn(const struct Imd *imd);
