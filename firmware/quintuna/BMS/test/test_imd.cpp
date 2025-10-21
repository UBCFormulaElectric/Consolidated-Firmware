#include "test_BMSBase.hpp"

extern "C"
{
#include "app/imd.h"
}

class ImdTest : public BMSBaseTest
{
};

TEST_F(ImdTest, check_insulation_resistance_for_normal_and_undervoltage_conditions)
{
    const std::vector<ImdConditionName> condition_names = { IMD_CONDITION_NORMAL, IMD_CONDITION_UNDERVOLTAGE_DETECTED };

    for (const auto &condition_name : condition_names)
    {
        SetImdCondition(condition_name);

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

TEST_F(ImdTest, check_good_and_bad_evaluation_for_sst_condition)
{
    SetImdCondition(IMD_CONDITION_SST);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 5...10% ("good")
    //                   90...95% ("bad")

    constexpr float GOOD_MIN_DUTY_CYCLE = 5.0f;
    constexpr float GOOD_MAX_DUTY_CYCLE = 10.0f;

    constexpr float BAD_MIN_DUTY_CYCLE = 90.0f;
    constexpr float BAD_MAX_DUTY_CYCLE = 95.0f;

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

TEST_F(ImdTest, check_pwm_encoding_for_device_error_condition)
{
    SetImdCondition(IMD_CONDITION_DEVICE_ERROR);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_pwm_encoding_for_ground_fault_condition)
{
    SetImdCondition(IMD_CONDITION_GROUND_FAULT);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_mapping_for_frequency_to_condition)
{
    struct ConditionLut
    {
        const float            frequency;
        const ImdConditionName condition_name;
    };

    static constexpr std::array<ConditionLut, 32> lookup_table = { {
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
    } };

    for (auto &[frequency, condition_name] : lookup_table)
    {
        fakes::imd::setFrequency(frequency);
        ASSERT_EQ(app_imd_getCondition().name, condition_name);
    }
}
