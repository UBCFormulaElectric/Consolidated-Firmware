#include "io_imd.h"
#include "app_imd.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "App_CanUtils.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"

#define IMD_FREQUENCY_TOLERANCE 2.0f

/**
 * Get the ideal frequency for the given IMD condition name
 * @param condition_name: The IMD condition name to look up ideal frequency for
 * @return The ideal frequency for the given IMD condition name
 */
static float getIdealPwmFrequency(ImdConditionName condition_name)
{
    assert(condition_name < NUM_OF_IMD_CONDITIONS);

    // Key: IMD condition
    // Value: PWM output frequency
    static const float imd_frequency_lookup[NUM_OF_IMD_CONDITIONS] = {
        [IMD_SHORT_CIRCUIT] = 0.0f, [IMD_NORMAL] = 10.0f,       [IMD_UNDERVOLTAGE_DETECTED] = 20.0f,
        [IMD_SST] = 30.0f,          [IMD_DEVICE_ERROR] = 40.0f, [IMD_GROUND_FAULT] = 50.0f,
    };

    return imd_frequency_lookup[condition_name];
}

/**
 * Convert the given PWM frequency to an IMD condition name. Note the PWM
 * frequency won't be exact so we must do some approximation.
 * @param frequency: The PWM frequency to convert to an IMD condition name
 * @return The IMD condition corresponding to the given PWM frequency
 */
static ImdConditionName estimateConditionName(const float frequency)
{
    ImdConditionName condition_name = IMD_INVALID;

    for (ImdConditionName i = 0U; i < NUM_OF_IMD_CONDITIONS; i++)
    {
        // Use min() because subtracting from 0Hz (IMD_SHORT_CIRCUIT) causes an
        // underflow
        const float lower_bound = MIN(getIdealPwmFrequency(i), getIdealPwmFrequency(i) - IMD_FREQUENCY_TOLERANCE);

        const float upper_bound = getIdealPwmFrequency(i) + IMD_FREQUENCY_TOLERANCE;

        if (frequency >= lower_bound && frequency <= upper_bound)
        {
            condition_name = i;
            break;
        }
    }

    return condition_name;
}

ImdCondition app_imd_getCondition()
{
    const float pwm_frequency  = app_imd_getPwmFrequency();
    const float pwm_duty_cycle = app_imd_getPwmDutyCycle();

    ImdCondition condition;
    memset(&condition, 0, sizeof(condition));

    condition.name = estimateConditionName(pwm_frequency);

    // Decode the information encoded in the PWM frequency and duty cycle
    switch (condition.name)
    {
        case IMD_SHORT_CIRCUIT:
        {
            // This condition doesn't use duty cycle to encode information so
            // any duty cycle is valid.
            condition.pwm_encoding.valid_duty_cycle = true;
        }
        break;
        case IMD_NORMAL:
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            condition.pwm_encoding.valid_duty_cycle =
                (pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 95.0f) ? true : false;

            if (condition.pwm_encoding.valid_duty_cycle)
            {
                if (pwm_duty_cycle == 5.0f)
                {
                    // The formula for calculating the insulation resistance
                    // causes a div-by-zero if the duty cycle is 5%.
                    // Curiously, 5% duty cycle is still defined as valid so
                    // we hard-code the resistance to be 50MOhms.
                    condition.pwm_encoding.insulation_measurement_dcp_kohms = 50000;
                }
                else
                {
                    // The insulation resistance is supposed to be saturate at
                    // 50MOhms, but the formula for calculating the insulation
                    // resistance exceeds 50Ohms once the duty cycle is below
                    // ~7.1%. Thus, we manually saturate the value at 50MOhms to
                    // get well-defined behaviours.
                    uint16_t resistance = (uint16_t)(1080.0f / (pwm_duty_cycle / 100.0f - 0.05f) - 1200.0f);

                    condition.pwm_encoding.insulation_measurement_dcp_kohms = MIN(resistance, 50000);
                }
            }
        }
        break;
        case IMD_SST:
        {
            condition.pwm_encoding.valid_duty_cycle = ((pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 10.0f) ||
                                                       (pwm_duty_cycle >= 90.0f && pwm_duty_cycle <= 95.0f))
                                                          ? true
                                                          : false;

            if (condition.pwm_encoding.valid_duty_cycle)
            {
                if (pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 10.0f)
                {
                    condition.pwm_encoding.speed_start_status = SST_GOOD;
                }
                else if (pwm_duty_cycle >= 90.0f && pwm_duty_cycle <= 95.0f)
                {
                    condition.pwm_encoding.speed_start_status = SST_BAD;
                }
            }
        }
        break;
        case IMD_DEVICE_ERROR:
        case IMD_GROUND_FAULT:
        {
            condition.pwm_encoding.valid_duty_cycle =
                (pwm_duty_cycle >= 47.5f && pwm_duty_cycle <= 52.5f) ? true : false;
        }
        break;
        case IMD_INVALID:
        {
            condition.pwm_encoding.valid_duty_cycle = false;
        }
        break;
        default:
        {
            condition.pwm_encoding.valid_duty_cycle = false;
        }
        break;
    }

    return condition;
}

float app_imd_getPwmFrequency()
{
    return io_imd_getFrequency();
}

float app_imd_getPwmDutyCycle()
{
    return io_imd_getDutyCycle();
}

void app_imd_broadcast()
{
    App_CanTx_BMS_ImdFrequency_Set(app_imd_getPwmFrequency());
    App_CanTx_BMS_ImdDutyCycle_Set(app_imd_getPwmDutyCycle());
    App_CanTx_BMS_ImdSecondsSincePowerOn_Set(io_imd_getTimeSincePowerOn());

    const ImdCondition condition = app_imd_getCondition();
    App_CanTx_BMS_ImdCondition_Set((uint8_t)condition.name);
    App_CanTx_BMS_ImdValidDutyCycle_Set(condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Set(
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMS_ImdActiveFrequency_Set(IMD_10Hz);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Set(
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMS_ImdActiveFrequency_Set(IMD_20Hz);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_BMS_ImdSpeedStartStatus30Hz_Set(condition.pwm_encoding.speed_start_status);
        }
        break;
        default:
        {
            // Nothing to do for conditions that don't carry a payload
        }
        break;
    }
}
