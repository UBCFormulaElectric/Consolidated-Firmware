#include <string.h>
#include <stdlib.h>

#include "App_Imd.h"
#include "App_SharedAssert.h"
#include "App_SharedMacros.h"

// States for speed start measurement
enum SST
{
    SST_GOOD,
    SST_BAD,
    SST_INVALID
};

struct Imd_PwmEncoding
{
    bool valid_duty_cycle;
    union {
        // 0Hz, 40Hz, 50Hz: PWM doesn't encode any information
        uint8_t dummy;
        // 10 and 20Hz: Insulation measurement DCP
        uint16_t insulation_measurement_dcp_kohms;
        // 30Hz: Speed Start Measurement
        enum SST speed_start_status;
    };
};

struct Imd
{
    float (*get_pwm_frequency)(void);
    float (*get_pwm_frequency_tolerance)(void);
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
    float (*const get_pwm_frequency_tolerance)(void),
    float (*const get_pwm_duty_cycle)(void),
    uint32_t (*const get_seconds_since_power_on)(void))
{
    shared_assert(get_pwm_frequency != NULL);
    shared_assert(get_pwm_frequency_tolerance != NULL);
    shared_assert(get_pwm_duty_cycle != NULL);
    shared_assert(get_seconds_since_power_on != NULL);

    struct Imd *imd = (struct Imd *)malloc(sizeof(struct Imd));
    shared_assert(imd != NULL);

    imd->get_pwm_frequency           = get_pwm_frequency;
    imd->get_pwm_frequency_tolerance = get_pwm_frequency_tolerance;
    imd->get_pwm_duty_cycle          = get_pwm_duty_cycle;
    imd->get_seconds_since_power_on  = get_seconds_since_power_on;

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
    imd->pwm_frequency           = imd->get_pwm_frequency();
    imd->pwm_frequency_tolerance = imd->get_pwm_frequency_tolerance();
    imd->pwm_duty_cycle          = imd->get_pwm_duty_cycle();
    imd->seconds_since_power_on  = imd->get_seconds_since_power_on();
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

// TODO: Uncomment and port this over to state machine once ready
/**
 * Set the periodic CAN signals related to IMD for the given CAN TX
 * interface
 * @param can_tx The CAN TX interface to update periodic CAN signals for
 * @param imd The IMD to update periodic CAN signals with
 */
// void App_RunState_SetImdPeriodicSignals(
//    struct CanTxInterface * can_tx,
//    const struct Imd *const imd);

// void App_RunState_SetImdPeriodicSignals(
//    struct CanTxInterface * can_tx,
//    const struct Imd *const imd)
//{
//    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(
//        can_tx, App_Imd_GetSecondsSincePowerOn(imd));
//    App_CanTx_SetPeriodicSignal_FREQUENCY(can_tx,
//    App_Imd_GetPwmFrequency(imd)); App_CanTx_SetPeriodicSignal_DUTY_CYCLE(
//        can_tx, App_Imd_GetPwmDutyCycle(imd));

//    const enum Imd_Condition condition = App_Imd_GetCondition(imd);
//    App_CanTx_SetPeriodicSignal_CONDITION(can_tx, condition);

//    const struct Imd_PwmEncoding pwm_encoding = App_Imd_GetPwmEncoding(imd);

//    App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
//        can_tx, pwm_encoding.valid_duty_cycle);

//    switch (condition)
//    {
//        case IMD_SHORT_CIRCUIT:
//        {
//            App_CanTx_SetPeriodicSignal_DUMMY_0_HZ(can_tx, 0);
//        }
//        break;
//        case IMD_NORMAL:
//        {
//            if (pwm_encoding.valid_duty_cycle == true)
//            {
//                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
//                    can_tx, pwm_encoding.insulation_measurement_dcp_kohms);
//            }
//        }
//        break;
//        case IMD_UNDERVOLTAGE_DETECTED:
//        {
//            if (pwm_encoding.valid_duty_cycle == true)
//            {
//                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
//                    can_tx, pwm_encoding.insulation_measurement_dcp_kohms);
//            }
//        }
//        break;
//        case IMD_SST:
//        {
//            App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
//                pwm_encoding.speed_start_status);
//        }
//        break;
//        case IMD_DEVICE_ERROR:
//        {
//            App_CanTx_SetPeriodicSignal_DUMMY_40_HZ(can_tx, 0);
//        }
//        break;
//        case IMD_EARTH_FAULT:
//        {
//            App_CanTx_SetPeriodicSignal_DUMMY_50_HZ(can_tx, 0);
//        }
//        break;
//        case IMD_INVALID:
//        {
//            App_CanTx_SetPeriodicSignal_DUMMY_INVALID(can_tx, 0);
//        }
//        break;
//    }
//}
