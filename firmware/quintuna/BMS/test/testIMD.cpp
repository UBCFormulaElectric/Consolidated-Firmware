#include "test_BMSBase.hpp"

extern "C"
{
#include "app_imd.h"
}
class BMSIMDTest : public BMSBaseTest
{
  public:
    static void test_imd_setImdCondition(const ImdConditionName condition_name)
    {
        const std::map<ImdConditionName, float> mapping{
            { IMD_CONDITION_SHORT_CIRCUIT, 0.0f },          { IMD_CONDITION_NORMAL, 10.0f },
            { IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f }, { IMD_CONDITION_SST, 30.0f },
            { IMD_CONDITION_DEVICE_ERROR, 40.0f },          { IMD_CONDITION_GROUND_FAULT, 50.0f }
        };
        fakes::imd::setFrequency(mapping.at(condition_name));
        ASSERT_EQ(condition_name, app_imd_getCondition().name);
    }
};

TEST_F(BMSIMDTest, check_insulation_resistance_for_normal_and_undervoltage_conditions)
{
    constexpr float IMD_CONDITION_NORMAL_FREQUENCY                = 10.0f;
    constexpr float IMD_CONDITION_UNDERVOLTAGE_DETECTED_FREQUENCY = 20.0f;

    for (const std::vector condition_frequencies = { IMD_CONDITION_NORMAL_FREQUENCY,
                                                     IMD_CONDITION_UNDERVOLTAGE_DETECTED_FREQUENCY };
         const auto        frequency : condition_frequencies)
    {
        fakes::imd::setFrequency(frequency);

        // From ISOMETER® IR155-3203/IR155-3204 manual:
        //     Insulation Resistance =
        //         (90% * 1200kOhm) / (duty_cycle - 5%) - 1200kOhm

        constexpr float MAX_VALID_DUTY_CYCLE = 95.0f;
        constexpr float MIN_VALID_DUTY_CYCLE = 5.0f;

        fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE - 0.01f);
        ImdCondition condition = app_imd_getCondition();
        ASSERT_EQ(false, condition.pwm_encoding.valid_duty_cycle);

        fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(50000, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fakes::imd::setDutyCycle((MIN_VALID_DUTY_CYCLE + MAX_VALID_DUTY_CYCLE) / 2.0f);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(1200, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(0, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE + 0.01f);
        ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
    }
}

TEST_F(BMSIMDTest, check_good_and_bad_evaluation_for_sst_condition)
{
    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 5...10% ("good")
    //                   90...95% ("bad")

    constexpr float SST_FREQUENCY = 30.0f;

    constexpr float GOOD_MIN_DUTY_CYCLE = 5.0f;
    constexpr float GOOD_MAX_DUTY_CYCLE = 10.0f;

    constexpr float BAD_MIN_DUTY_CYCLE = 90.0f;
    constexpr float BAD_MAX_DUTY_CYCLE = 95.0f;

    fakes::imd::setFrequency(SST_FREQUENCY);

    fakes::imd::setDutyCycle(GOOD_MIN_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(GOOD_MIN_DUTY_CYCLE);
    ImdCondition condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    fakes::imd::setDutyCycle(GOOD_MAX_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    fakes::imd::setDutyCycle(GOOD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle((GOOD_MIN_DUTY_CYCLE + BAD_MIN_DUTY_CYCLE) / 2.0f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(BAD_MIN_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(BAD_MIN_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    fakes::imd::setDutyCycle(BAD_MAX_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    fakes::imd::setDutyCycle(BAD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(BMSIMDTest, check_pwm_encoding_for_device_error_condition)
{
    test_imd_setImdCondition(IMD_CONDITION_DEVICE_ERROR);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float DEVICE_ERROR_FREQUENCY = 40.0f;

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    fakes::imd::setFrequency(DEVICE_ERROR_FREQUENCY);

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(BMSIMDTest, check_mapping_for_frequency_to_condition)
{
    struct ConditionLut
    {
        float            frequency;
        ImdConditionName condition_name;
    };

    const std::vector<ConditionLut> lookup_table = {
        { 0.0f, IMD_CONDITION_SHORT_CIRCUIT },
        { 1.0f, IMD_CONDITION_SHORT_CIRCUIT },
        { 2.0f, IMD_CONDITION_SHORT_CIRCUIT },
        { 3.0f, IMD_CONDITION_INVALID },
        { 17.0f, IMD_CONDITION_INVALID },
        { 18.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED },
        { 19.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED },
        { 20.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED },
        { 21.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED },
        { 22.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED },
        { 23.0f, IMD_CONDITION_INVALID },
        { 27.0f, IMD_CONDITION_INVALID },
        { 28.0f, IMD_CONDITION_SST },
        { 29.0f, IMD_CONDITION_SST },
        { 30.0f, IMD_CONDITION_SST },
        { 31.0f, IMD_CONDITION_SST },
        { 32.0f, IMD_CONDITION_SST },
        { 33.0f, IMD_CONDITION_INVALID },
        { 37.0f, IMD_CONDITION_INVALID },
        { 38.0f, IMD_CONDITION_DEVICE_ERROR },
        { 39.0f, IMD_CONDITION_DEVICE_ERROR },
        { 40.0f, IMD_CONDITION_DEVICE_ERROR },
        { 41.0f, IMD_CONDITION_DEVICE_ERROR },
        { 42.0f, IMD_CONDITION_DEVICE_ERROR },
        { 43.0f, IMD_CONDITION_INVALID },
        { 47.0f, IMD_CONDITION_INVALID },
        { 48.0f, IMD_CONDITION_GROUND_FAULT },
        { 49.0f, IMD_CONDITION_GROUND_FAULT },
        { 50.0f, IMD_CONDITION_GROUND_FAULT },
        { 51.0f, IMD_CONDITION_GROUND_FAULT },
        { 52.0f, IMD_CONDITION_GROUND_FAULT },
        { 53.0f, IMD_CONDITION_INVALID },
    };

    for (const auto &[frequency, condition_name] : lookup_table)
    {
        fakes::imd::setFrequency(frequency);
        ASSERT_EQ(app_imd_getCondition().name, condition_name);
    }
}