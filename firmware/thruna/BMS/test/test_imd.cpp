#include "test_imd.h"

TEST_F(ImdTest, check_insulation_resistance_for_normal_and_undervoltage_conditions)
{
    std::vector<ImdConditionName> condition_names = { IMD_NORMAL, IMD_UNDERVOLTAGE_DETECTED };

    for (auto& condition_name : condition_names)
    {
        test_imd_setImdCondition(condition_name);

        // From ISOMETER® IR155-3203/IR155-3204 manual:
        //     Insulation Resistance =
        //         (90% * 1200kOhm) / (duty_cycle - 5%) - 1200kOhm

        constexpr float MAX_VALID_DUTY_CYCLE = 95.0f;
        constexpr float MIN_VALID_DUTY_CYCLE = 5.0f;

        fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE - 0.01f);
        ImdCondition condition = app_imd_getCondition();
        ASSERT_EQ(false, condition.pwm_encoding.valid_duty_cycle);

        fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(50000, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fake_io_imd_getDutyCycle_returns((MIN_VALID_DUTY_CYCLE + MAX_VALID_DUTY_CYCLE) / 2.0f);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(1200, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE);
        condition = app_imd_getCondition();
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(0, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE + 0.01f);
        ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
    }
}

TEST_F(ImdTest, check_good_and_bad_evaluation_for_sst_condition)
{
    test_imd_setImdCondition(IMD_SST);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 5...10% ("good")
    //                   90...95% ("bad")

    constexpr float GOOD_MIN_DUTY_CYCLE = 5.0f;
    constexpr float GOOD_MAX_DUTY_CYCLE = 10.0f;

    constexpr float BAD_MIN_DUTY_CYCLE = 90.0f;
    constexpr float BAD_MAX_DUTY_CYCLE = 95.0f;

    fake_io_imd_getDutyCycle_returns(GOOD_MIN_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(GOOD_MIN_DUTY_CYCLE);
    ImdCondition condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    fake_io_imd_getDutyCycle_returns(GOOD_MAX_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    fake_io_imd_getDutyCycle_returns(GOOD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns((GOOD_MIN_DUTY_CYCLE + BAD_MIN_DUTY_CYCLE) / 2.0f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(BAD_MIN_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(BAD_MIN_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    fake_io_imd_getDutyCycle_returns(BAD_MAX_DUTY_CYCLE);
    condition = app_imd_getCondition();
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    fake_io_imd_getDutyCycle_returns(BAD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_pwm_encoding_for_device_error_condition)
{
    test_imd_setImdCondition(IMD_DEVICE_ERROR);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_pwm_encoding_for_ground_fault_condition)
{
    test_imd_setImdCondition(IMD_GROUND_FAULT);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MIN_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE);
    ASSERT_EQ(true, app_imd_getCondition().pwm_encoding.valid_duty_cycle);

    fake_io_imd_getDutyCycle_returns(MAX_VALID_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app_imd_getCondition().pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_mapping_for_frequency_to_condition)
{
    struct ConditionLut
    {
        float            frequency;
        ImdConditionName condition_name;
    };

    std::vector<struct ConditionLut> lookup_table = {
        { 0.0f, IMD_SHORT_CIRCUIT },
        { 1.0f, IMD_SHORT_CIRCUIT },
        { 2.0f, IMD_SHORT_CIRCUIT },
        { 3.0f, IMD_INVALID },
        { 17.0f, IMD_INVALID },
        { 18.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 19.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 20.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 21.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 22.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 23.0f, IMD_INVALID },
        { 27.0f, IMD_INVALID },
        { 28.0f, IMD_SST },
        { 29.0f, IMD_SST },
        { 30.0f, IMD_SST },
        { 31.0f, IMD_SST },
        { 32.0f, IMD_SST },
        { 33.0f, IMD_INVALID },
        { 37.0f, IMD_INVALID },
        { 38.0f, IMD_DEVICE_ERROR },
        { 39.0f, IMD_DEVICE_ERROR },
        { 40.0f, IMD_DEVICE_ERROR },
        { 41.0f, IMD_DEVICE_ERROR },
        { 42.0f, IMD_DEVICE_ERROR },
        { 43.0f, IMD_INVALID },
        { 47.0f, IMD_INVALID },
        { 48.0f, IMD_GROUND_FAULT },
        { 49.0f, IMD_GROUND_FAULT },
        { 50.0f, IMD_GROUND_FAULT },
        { 51.0f, IMD_GROUND_FAULT },
        { 52.0f, IMD_GROUND_FAULT },
        { 53.0f, IMD_INVALID },
    };

    for (auto& entry : lookup_table)
    {
        fake_io_imd_getFrequency_returns(entry.frequency);
        ASSERT_EQ(app_imd_getCondition().name, entry.condition_name);
    }
}
