#include <string.h>
#include <stdlib.h>

#include "auto_generated/App_CanMsgs.h"
#include "App_Imd.h"
#include "App_SharedAssert.h"
#include "App_SharedMacros.h"

// Match the IMD enums with the DBC multiplexer values of the IMD message
static_assert(
    IMD_SHORT_CIRCUIT == CANMSGS_BMS_IMD_CONDITION_IMD_SHORT_CIRCUIT_CHOICE,
    "The IMD short circuit enum must match its DBC multiplexer value");
static_assert(
    IMD_NORMAL == CANMSGS_BMS_IMD_CONDITION_IMD_NORMAL_CHOICE,
    "The IMD normal enum must match its DBC multiplexer value");
static_assert(
    IMD_UNDERVOLTAGE_DETECTED ==
        CANMSGS_BMS_IMD_CONDITION_IMD_UNDERVOLTAGE_DETECTED_CHOICE,
    "The IMD undervoltage detected enum must match its DBC multiplexer value");
static_assert(
    IMD_SST == CANMSGS_BMS_IMD_CONDITION_IMD_SST_CHOICE,
    "The IMD speed start measurement enum must match its DBC multiplexer "
    "value");
static_assert(
    IMD_DEVICE_ERROR == CANMSGS_BMS_IMD_CONDITION_IMD_DEVICE_ERROR_CHOICE,
    "The IMD device error enum must match its DBC multiplexer value");
static_assert(
    IMD_EARTH_FAULT == CANMSGS_BMS_IMD_CONDITION_IMD_EARTH_FAULT_CHOICE,
    "The IMD earth fault enum must match its DBC multiplexer value");

struct Imd
{
    float (*get_pwm_frequency)(void);
    float (*get_pwm_duty_cycle)(void);
    uint32_t (*get_seconds_since_power_on)(void);

    float                  pwm_frequency;
    float                  pwm_frequency_tolerance;
    float                  pwm_duty_cycle;
    uint32_t               seconds_since_power_on;
    enum Imd_Condition     condition;
    struct Imd_PwmEncoding pwm_encoding;
};

/**
 * Convert the given PWM frequency to an IMD condition. Note the PWM frequency
 * won't be exact so we must do some approximation.
 * @param frequency: The PWM frequency to convert to an IMD condition
 * @param tolerance: The tolerance allowed in the PWM frequency
 * @return The IMD condition corresponding to the given PWM frequency
 */
static enum Imd_Condition
    App_EstimateCondition(float frequency, float tolerance);

/**
 * Get the ideal frequency for the given IMD condition
 * @param condition: The IMD condition to look up ideal frequency for
 * @return The ideal frequency for the given IMD condition
 */
static float App_GetIdealPwmFrequency(enum Imd_Condition condition);

static enum Imd_Condition
    App_EstimateCondition(const float frequency, const float tolerance)
{
    enum Imd_Condition condition = IMD_INVALID;

    for (enum Imd_Condition i = 0U; i < NUM_OF_IMD_CONDITIONS; i++)
    {
        // Use min() because subtracting from 0Hz (IMD_SHORT_CIRCUIT) causes an
        // underflow
        const float lower_bound =
            min(App_GetIdealPwmFrequency(i),
                App_GetIdealPwmFrequency(i) - tolerance);

        const float upper_bound = App_GetIdealPwmFrequency(i) + tolerance;

        if (frequency >= lower_bound && frequency <= upper_bound)
        {
            condition = i;
            break;
        }
    }

    return condition;
}

static float App_GetIdealPwmFrequency(const enum Imd_Condition condition)
{
    shared_assert(condition < NUM_OF_IMD_CONDITIONS);

    // Key: IMD condition
    // Value: PWM output frequency
    static const uint32_t imd_frequency_lookup[NUM_OF_IMD_CONDITIONS] = {
        [IMD_SHORT_CIRCUIT] = 0U,          [IMD_NORMAL] = 10U,
        [IMD_UNDERVOLTAGE_DETECTED] = 20U, [IMD_SST] = 30U,
        [IMD_DEVICE_ERROR] = 40U,          [IMD_EARTH_FAULT] = 50U,
    };

    return imd_frequency_lookup[condition];
}

struct Imd *App_Imd_Create(
    float (*const get_pwm_frequency)(void),
    const float pwm_frequency_tolerance,
    float (*const get_pwm_duty_cycle)(void),
    uint32_t (*const get_seconds_since_power_on)(void))
{
    shared_assert(get_pwm_frequency != NULL);
    shared_assert(get_pwm_duty_cycle != NULL);
    shared_assert(get_seconds_since_power_on != NULL);

    struct Imd *imd = (struct Imd *)malloc(sizeof(struct Imd));
    shared_assert(imd != NULL);

    imd->get_pwm_frequency          = get_pwm_frequency;
    imd->pwm_frequency_tolerance    = pwm_frequency_tolerance;
    imd->get_pwm_duty_cycle         = get_pwm_duty_cycle;
    imd->get_seconds_since_power_on = get_seconds_since_power_on;

    memset(&imd->pwm_encoding, 0, sizeof(imd->pwm_encoding));

    return imd;
}

void App_Imd_Destroy(struct Imd *const imd)
{
    free(imd);
}

void App_Imd_Tick(struct Imd *const imd)
{
    // Update internal state at the start of each tick
    imd->pwm_frequency          = imd->get_pwm_frequency();
    imd->pwm_duty_cycle         = imd->get_pwm_duty_cycle();
    imd->seconds_since_power_on = imd->get_seconds_since_power_on();
    imd->condition =
        App_EstimateCondition(imd->pwm_frequency, imd->pwm_frequency_tolerance);

    // Decode the information encoded in the PWM frequency and duty cycle
    switch (imd->condition)
    {
        case IMD_SHORT_CIRCUIT:
        {
            // This condition doesn't use duty cycle to encode information so
            // any duty cycle is valid.
            imd->pwm_encoding.valid_duty_cycle = true;
            imd->pwm_encoding.dummy            = 0;
        }
        break;
        case IMD_NORMAL:
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            imd->pwm_encoding.valid_duty_cycle =
                (imd->pwm_duty_cycle > 5.0f && imd->pwm_duty_cycle < 95.0f)
                    ? true
                    : false;

            if (imd->pwm_encoding.valid_duty_cycle)
            {
                imd->pwm_encoding.insulation_measurement_dcp_kohms =
                    1080.0f / (imd->pwm_duty_cycle / 100.0f - 0.05f) - 1200.0f;
            }
        }
        break;
        case IMD_SST:
        {
            imd->pwm_encoding.valid_duty_cycle =
                ((imd->pwm_duty_cycle > 5.0f && imd->pwm_duty_cycle < 10.0f) ||
                 (imd->pwm_duty_cycle > 90.0f && imd->pwm_duty_cycle < 95.0f))
                    ? true
                    : false;

            if (imd->pwm_encoding.valid_duty_cycle)
            {
                if (imd->pwm_duty_cycle > 5.0f && imd->pwm_duty_cycle < 10.0f)
                {
                    imd->pwm_encoding.speed_start_status = SST_GOOD;
                }
                else if (
                    imd->pwm_duty_cycle > 90.0f && imd->pwm_duty_cycle < 95.0f)
                {
                    imd->pwm_encoding.speed_start_status = SST_BAD;
                }
            }
        }
        break;
        case IMD_DEVICE_ERROR:
        case IMD_EARTH_FAULT:
        {
            imd->pwm_encoding.valid_duty_cycle =
                (imd->pwm_duty_cycle > 47.5f && imd->pwm_duty_cycle < 52.5f)
                    ? true
                    : false;
            imd->pwm_encoding.dummy = 0;
        }
        break;
        case IMD_INVALID:
        {
            imd->pwm_encoding.valid_duty_cycle = false;
            imd->pwm_encoding.dummy            = 0;
        }
        break;
        default:
        {
            imd->pwm_encoding.valid_duty_cycle = false;
            imd->pwm_encoding.dummy            = 0;
        }
        break;
    }
}

float App_Imd_GetPwmFrequency(const struct Imd *const imd)
{
    return imd->pwm_frequency;
}

float App_Imd_GetPwmDutyCycle(const struct Imd *const imd)
{
    return imd->pwm_duty_cycle;
}

uint32_t App_Imd_GetSecondsSincePowerOn(const struct Imd *const imd)
{
    return imd->seconds_since_power_on;
}

enum Imd_Condition App_Imd_GetCondition(const struct Imd *const imd)
{
    return imd->condition;
}

struct Imd_PwmEncoding App_Imd_GetPwmEncoding(const struct Imd *const imd)
{
    return imd->pwm_encoding;
}
