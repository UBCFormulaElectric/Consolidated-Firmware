#include "auto_generated/App_CanTx.h"
#include "App_Imd.h"
#include "World/App_SharedWorld.h"
#include "Io_SharedAssert.h"
#include "Io_SharedMacros.h"

extern struct World *world;

// We only require one IMD per vehicle
#define MAX_NUM_OF_IMDS 1

// The frequency of the IMD's PWM output won't be exact, so need some tolerance
// in our measurements.
#define CONDITION_FREQUENCY_TOLERANCE 2U // +/- Hz

// States for speed start measurement
enum SST
{
    SST_GOOD,
    SST_BAD,
    SST_INVALID
};

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

struct Imd
{
    float (*get_pwm_frequency)(void);
    float (*get_pwm_duty_cycle)(void);
    uint32_t (*get_seconds_since_power_on)(void);
    struct
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
    } pwm_encoding;
};

/**
 * Convert the given PWM frequency to an IMD condition. Note the PWM frequency
 * won't be exact so we must do some approximation.
 * @param frequency: The PWM frequency to convert to an IMD condition
 * @return The IMD condition corresponding to the given PWM frequency
 */
static enum Imd_Condition App_EstimateCondition(const float frequency);

/**
 * Get the ideal frequency for the given IMD condition
 * @param condition: The IMD condition to look up ideal frequency for
 * @return The ideal frequency for the given IMD condition
 */
static uint32_t App_GetIdealPwmFrequency(const enum Imd_Condition condition);

static enum Imd_Condition App_EstimateCondition(const float frequency)
{
    enum Imd_Condition condition = IMD_INVALID;

    for (enum Imd_Condition i = 0U; i < NUM_OF_IMD_CONDITIONS; i++)
    {
        // Use min() because subtracting from 0Hz (IMD_SHORT_CIRCUIT) causes an
        // underflow
        const uint32_t lower_bound =
            min(App_GetIdealPwmFrequency(i),
                App_GetIdealPwmFrequency(i) - CONDITION_FREQUENCY_TOLERANCE);

        const uint32_t upper_bound =
            App_GetIdealPwmFrequency(i) + CONDITION_FREQUENCY_TOLERANCE;

        if (frequency > lower_bound && frequency < upper_bound)
        {
            condition = i;
            break;
        }
    }

    return condition;
}

static uint32_t App_GetIdealPwmFrequency(const enum Imd_Condition condition)
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
    float (*get_pwm_frequency)(void),
    float (*get_pwm_duty_cycle)(void),
    uint32_t (*get_seconds_since_power_on)(void))
{
    shared_assert(get_pwm_frequency != NULL);
    shared_assert(get_pwm_duty_cycle != NULL);
    shared_assert(get_seconds_since_power_on != NULL);

    static struct Imd imds[MAX_NUM_OF_IMDS];
    static size_t     alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_IMDS);

    struct Imd *const imd = &imds[alloc_index++];

    imd->get_pwm_frequency          = get_pwm_frequency;
    imd->get_pwm_duty_cycle         = get_pwm_duty_cycle;
    imd->get_seconds_since_power_on = get_seconds_since_power_on;

    imd->pwm_encoding.valid_duty_cycle = false;
    imd->pwm_encoding.dummy            = 0.0f;

    return imd;
}

void App_Imd_Tick(struct Imd *const imd)
{
    shared_assert(imd != NULL);

    // The IMD takes a short while before its values are stabilized, so it is
    // useful to keep track of how long it's been since the IMD powered on.
    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(
        App_SharedWorld_GetCanTx(world), imd->get_seconds_since_power_on());

    // Use the same frequency and duty cycle throughout the entire tick
    const float frequency  = imd->get_pwm_frequency();
    const float duty_cycle = imd->get_pwm_duty_cycle();
    App_CanTx_SetPeriodicSignal_FREQUENCY(
        App_SharedWorld_GetCanTx(world), frequency);
    App_CanTx_SetPeriodicSignal_DUTY_CYCLE(
        App_SharedWorld_GetCanTx(world), duty_cycle);

    // Map the PWM frequency to the appropriate IMD condition
    enum Imd_Condition condition = App_EstimateCondition(frequency);
    App_CanTx_SetPeriodicSignal_CONDITION(
        App_SharedWorld_GetCanTx(world), condition);

    // Decode the information encoded in the PWM frequency and duty cycle
    switch (condition)
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
                (duty_cycle > 5.0f && duty_cycle < 95.0f) ? true : false;
            App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
                App_SharedWorld_GetCanTx(world),
                imd->pwm_encoding.valid_duty_cycle);

            if (imd->pwm_encoding.valid_duty_cycle)
            {
                const uint16_t resistance_kohms =
                    1080.0f / (duty_cycle / 100.0f - 0.05f) - 1200.0f;
                imd->pwm_encoding.insulation_measurement_dcp_kohms =
                    resistance_kohms;

                if (condition == IMD_NORMAL)
                {
                    App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                        App_SharedWorld_GetCanTx(world), resistance_kohms);
                }
                else if (condition == IMD_UNDERVOLTAGE_DETECTED)
                {
                    App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                        App_SharedWorld_GetCanTx(world), resistance_kohms);
                }
            }
        }
        break;
        case IMD_SST:
        {
            imd->pwm_encoding.valid_duty_cycle =
                ((duty_cycle > 5.0f && duty_cycle < 10.0f) ||
                 (duty_cycle > 90.0f && duty_cycle < 95.0f))
                    ? true
                    : false;
            App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
                App_SharedWorld_GetCanTx(world),
                imd->pwm_encoding.valid_duty_cycle);

            if (imd->pwm_encoding.valid_duty_cycle)
            {
                if (duty_cycle > 5.0f && duty_cycle < 10.0f)
                {
                    enum SST status                      = SST_GOOD;
                    imd->pwm_encoding.speed_start_status = status;
                    App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                        App_SharedWorld_GetCanTx(world), status);
                }
                else if (duty_cycle > 90.0f && duty_cycle < 95.0f)
                {
                    enum SST status                      = SST_BAD;
                    imd->pwm_encoding.speed_start_status = status;
                    App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                        App_SharedWorld_GetCanTx(world), status);
                }
            }
        }
        break;
        case IMD_DEVICE_ERROR:
        case IMD_EARTH_FAULT:
        {
            imd->pwm_encoding.valid_duty_cycle =
                (duty_cycle > 47.5f && duty_cycle < 52.5f) ? true : false;
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
