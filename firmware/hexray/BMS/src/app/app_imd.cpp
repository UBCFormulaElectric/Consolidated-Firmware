#include "app_imd.hpp"
#include "io_imd.hpp"
#include <cassert>
#include <cmath>
#include <algorithm>
#include "app_canUtils.hpp"
#include "app_canTx.hpp"

constexpr int   NUM_OF_IMD_CONDITIONS   = 6;
constexpr float IMD_FREQUENCY_TOLERANCE = 2.0;
constexpr float IMD_FREQUENCY_LOOKUP[]  = {
    0.0f,  // IMD_CONDITION_SHORT_CIRCUIT
    10.0f, // IMD_CONDITION_NORMAL
    20.0f, // IMD_CONDITION_UNDERVOLTAGE_DETECTED
    30.0f, // IMD_CONDITION_SST
    40.0f, // IMD_CONDITION_DEVICE_ERROR
    50.0f  // IMD_CONDITION_GROUND_FAULT
};

namespace app::imd
{

/**
 * Get the ideal frequency for the given IMD condition name
 * @param condition_name: The IMD condition name to look up ideal frequency for
 * @return The ideal frequency for the given IMD condition name
 */
static float getIdealPwmFrequency(const app::can_utils::ImdConditionName condition_name)
{
    assert(static_cast<int>(condition_name) >= 0 && static_cast<int>(condition_name) < NUM_OF_IMD_CONDITIONS);
    return IMD_FREQUENCY_LOOKUP[static_cast<int>(condition_name)];
}

/**
 * Convert the given PWM frequency to an IMD condition name. Note the PWM
 * frequency won't be exact so we must do some approximation.
 * @param frequency: The PWM frequency to convert to an IMD condition name
 * @return The IMD condition corresponding to the given PWM frequency
 */
static app::can_utils::ImdConditionName estimateConditionName(const float frequency)
{
    app::can_utils::ImdConditionName condition_name = app::can_utils::ImdConditionName::IMD_CONDITION_INVALID;
    for (int i = 0; i < NUM_OF_IMD_CONDITIONS; i++)
    {
        app::can_utils::ImdConditionName condition   = static_cast<app::can_utils::ImdConditionName>(i);
        const float                      ideal       = getIdealPwmFrequency(condition);
        const float                      lower_bound = fmaxf(0.0f, ideal - IMD_FREQUENCY_TOLERANCE);
        const float                      upper_bound = ideal + IMD_FREQUENCY_TOLERANCE;

        if (frequency >= lower_bound && frequency <= upper_bound)
        {
            condition_name = condition;
            break;
        }
    }
    return condition_name;
}

ImdCondition getCondition()
{
    const float pwm_frequency  = io::imd::getFrequency();
    const float pwm_duty_cycle = io::imd::getDutyCycle();

    ImdCondition condition{};
    condition.name = (pwm_frequency < 0.001f) ? app::can_utils::ImdConditionName::IMD_CONDITION_SHORT_CIRCUIT
                                              : estimateConditionName(pwm_frequency);

    // Decode the information encoded in the PWM frequency and duty cycle
    switch (condition.name)
    {
        case app::can_utils::ImdConditionName::IMD_CONDITION_SHORT_CIRCUIT:
        {
            // This condition doesn't use duty cycle to encode information so any duty cycle is valid.
            condition.valid_duty_cycle = true;
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_NORMAL:
        case app::can_utils::ImdConditionName::IMD_CONDITION_UNDERVOLTAGE_DETECTED:
        {
            condition.valid_duty_cycle = (pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 90.0f);
            if (condition.valid_duty_cycle)
            {
                if (pwm_duty_cycle - 5.0f < 0.001f)
                {
                    condition.payload.insulation_measurement_dcp_kohms = 50000;
                }
                else
                {
                    const uint16_t resistance =
                        static_cast<uint16_t>((1080.0f / (pwm_duty_cycle / 100.0f - 0.05f)) - 1200.0f);
                    condition.payload.insulation_measurement_dcp_kohms =
                        std::min(resistance, static_cast<uint16_t>(50000));
                }
            }
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_SST:
        {
            condition.valid_duty_cycle = ((pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 10.0f) ||
                                          (pwm_duty_cycle >= 90.0f && pwm_duty_cycle <= 95.0f))
                                             ? true
                                             : false;

            if (condition.valid_duty_cycle)
            {
                if (pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 10.0f)
                {
                    condition.payload.speed_start_status = ImdSst::GOOD;
                }
                else if (pwm_duty_cycle >= 90.0f && pwm_duty_cycle <= 95.0f)
                {
                    condition.payload.speed_start_status = ImdSst::BAD;
                }
            }
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_DEVICE_ERROR:
        case app::can_utils::ImdConditionName::IMD_CONDITION_GROUND_FAULT:
        {
            condition.valid_duty_cycle = (pwm_duty_cycle >= 47.5f && pwm_duty_cycle <= 52.5f) ? true : false;
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_INVALID:
        default:
        {
            condition.valid_duty_cycle = false;
        }
    }
    return condition;
}

void broadcast()
{
    app::can_tx::BMS_ImdFrequency_set(io::imd::getFrequency());
    app::can_tx::BMS_ImdDutyCycle_set(io::imd::getDutyCycle());
    app::can_tx::BMS_ImdTimeSincePowerOn_set(static_cast<float>(io::imd::getTimeSincePowerOn()));

    const ImdCondition condition = app::imd::getCondition();
    app::can_tx::BMS_ImdCondition_set(condition.name);
    app::can_tx::BMS_ImdValidDutyCycle_set(condition.valid_duty_cycle);

    switch (condition.name)
    {
        case app::can_utils::ImdConditionName::IMD_CONDITION_SHORT_CIRCUIT:
        {
            app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_0Hz);
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_NORMAL:
        {
            if (condition.valid_duty_cycle)
            {
                app::can_tx::BMS_ImdInsulationMeasurementDcp10Hz_set(
                    condition.payload.insulation_measurement_dcp_kohms);
                app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_10Hz);
            }
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_UNDERVOLTAGE_DETECTED:
        {
            if (condition.valid_duty_cycle)
            {
                app::can_tx::BMS_ImdInsulationMeasurementDcp20Hz_set(
                    condition.payload.insulation_measurement_dcp_kohms);
                app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_20Hz);
            }
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_SST:
        {
            app::can_tx::BMS_ImdSpeedStartStatus30Hz_set((float)condition.payload.speed_start_status);
            app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_30Hz);
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_DEVICE_ERROR:
        {
            app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_40Hz);
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_GROUND_FAULT:
        {
            app::can_tx::BMS_ImdActiveFrequency_set(app::can_utils::ImdActiveFrequency::IMD_50Hz);
        }
        break;
        case app::can_utils::ImdConditionName::IMD_CONDITION_INVALID:
        default:
        {
            // Do nothing
        }
        break;
    }
}
} // namespace app::imd
