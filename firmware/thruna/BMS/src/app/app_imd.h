#pragma once

#include <stdint.h>
#include <stdbool.h>

// States for speed start measurement
typedef enum
{
    SST_GOOD,
    SST_BAD,
    SST_INVALID
} ImdSst;

typedef enum
{
    IMD_SHORT_CIRCUIT,
    IMD_NORMAL,
    IMD_UNDERVOLTAGE_DETECTED,
    IMD_SST,
    IMD_DEVICE_ERROR,
    IMD_GROUND_FAULT,
    NUM_OF_IMD_CONDITIONS,
    IMD_INVALID,
} ImdConditionName;

// The IMD has an PWM output that encodes information about the IMD condition:
//   - The frequency encodes a name
//   - The duty cycle encodes additional information (where applicable)
typedef struct
{
    ImdConditionName name;
    struct
    {
        bool valid_duty_cycle;
        union
        {
            // 10 and 20Hz: Insulation measurement DCP
            uint16_t insulation_measurement_dcp_kohms;
            // 30Hz: Speed Start Measurement
            ImdSst speed_start_status;
            // 0Hz, 40Hz, 50Hz: PWM doesn't encode any information
        };
    } pwm_encoding;
} ImdCondition;

/**
 * Get the condition for the given IMD
 * @param imd The IMD to get condition for
 * @return The condition for the given IMD
 */
ImdCondition app_imd_getCondition(void);

/**
 * Get the PWM frequency for the given IMD
 * @param imd The IMD to get PWM frequency for
 * @return The PWM frequency for the given IMD
 */
float app_imd_getPwmFrequency(void);

/**
 * Get the PWM duty cycle for the given IMD
 * @param imd The IMD to get PWM duty cycle for
 * @return The PWM duty cycle for the given IMD
 */
float app_imd_getPwmDutyCycle(void);

/**
 * Broadcast state of the IMD over the CAN bus.
 */
void app_imd_broadcast(void);
