#include "Test_Imd.h"

FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint16_t, get_seconds_since_power_on);

void ImdTest::SetImdCondition(
    struct Imd *           imd_to_set,
    enum Imd_ConditionName condition_name,
    float &                fake_pwm_frequency_return_val)
{
    const float mapping[NUM_OF_IMD_CONDITIONS] = {
        [IMD_SHORT_CIRCUIT] = 0.0f, [IMD_NORMAL] = 10.0f,       [IMD_UNDERVOLTAGE_DETECTED] = 20.0f,
        [IMD_SST] = 30.0f,          [IMD_DEVICE_ERROR] = 40.0f, [IMD_EARTH_FAULT] = 50.0f,
    };

    fake_pwm_frequency_return_val = mapping[condition_name];
    ASSERT_EQ(condition_name, App_Imd_GetCondition(imd_to_set).name);
}

void ImdTest::SetPwmFrequencyTolerance(struct Imd *&imd_to_set, float tolerance)
{
    TearDownObject(imd_to_set, App_Imd_Destroy);
    imd_to_set = App_Imd_Create(get_pwm_frequency, tolerance, get_pwm_duty_cycle, get_seconds_since_power_on);
}

void ImdTest::SetUp()
{
    constexpr float DEFAULT_FREQUENCY_TOLERANCE = 2.0f;

    imd =
        App_Imd_Create(get_pwm_frequency, DEFAULT_FREQUENCY_TOLERANCE, get_pwm_duty_cycle, get_seconds_since_power_on);

    RESET_FAKE(get_pwm_frequency);
    RESET_FAKE(get_pwm_duty_cycle);
    RESET_FAKE(get_seconds_since_power_on);
}

void ImdTest::TearDown()
{
    TearDownObject(imd, App_Imd_Destroy);
}

TEST_F(ImdTest, check_insulation_resistance_for_normal_and_undervoltage_conditions)
{
    std::vector<enum Imd_ConditionName> condition_names = { IMD_NORMAL, IMD_UNDERVOLTAGE_DETECTED };

    for (auto &condition_name : condition_names)
    {
        SetImdCondition(imd, condition_name, get_pwm_frequency_fake.return_val);

        // From ISOMETER® IR155-3203/IR155-3204 manual:
        //     Insulation Resistance =
        //         (90% * 1200kOhm) / (duty_cycle - 5%) - 1200kOhm

        constexpr float MAX_VALID_DUTY_CYCLE = 95.0f;
        constexpr float MIN_VALID_DUTY_CYCLE = 5.0f;

        get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE - 0.01f;
        struct Imd_Condition condition     = App_Imd_GetCondition(imd);
        ASSERT_EQ(false, condition.pwm_encoding.valid_duty_cycle);

        get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE;
        condition                          = App_Imd_GetCondition(imd);
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(50000, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        get_pwm_duty_cycle_fake.return_val = (MIN_VALID_DUTY_CYCLE + MAX_VALID_DUTY_CYCLE) / 2.0f;
        condition                          = App_Imd_GetCondition(imd);
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(1200, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE;
        condition                          = App_Imd_GetCondition(imd);
        ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
        ASSERT_EQ(0, condition.pwm_encoding.insulation_measurement_dcp_kohms);

        get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE + 0.01f;
        ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);
    }
}

TEST_F(ImdTest, check_good_and_bad_evaluation_for_sst_condition)
{
    SetImdCondition(imd, IMD_SST, get_pwm_frequency_fake.return_val);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 5...10% ("good")
    //                   90...95% ("bad")

    constexpr float GOOD_MIN_DUTY_CYCLE = 5.0f;
    constexpr float GOOD_MAX_DUTY_CYCLE = 10.0f;

    constexpr float BAD_MIN_DUTY_CYCLE = 90.0f;
    constexpr float BAD_MAX_DUTY_CYCLE = 95.0f;

    get_pwm_duty_cycle_fake.return_val = GOOD_MIN_DUTY_CYCLE - 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = GOOD_MIN_DUTY_CYCLE;
    struct Imd_Condition condition     = App_Imd_GetCondition(imd);
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    get_pwm_duty_cycle_fake.return_val = GOOD_MAX_DUTY_CYCLE;
    condition                          = App_Imd_GetCondition(imd);
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_GOOD, condition.pwm_encoding.speed_start_status);

    get_pwm_duty_cycle_fake.return_val = GOOD_MAX_DUTY_CYCLE + 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = (GOOD_MIN_DUTY_CYCLE + BAD_MIN_DUTY_CYCLE) / 2.0f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = BAD_MIN_DUTY_CYCLE - 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = BAD_MIN_DUTY_CYCLE;
    condition                          = App_Imd_GetCondition(imd);
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    get_pwm_duty_cycle_fake.return_val = BAD_MAX_DUTY_CYCLE;
    condition                          = App_Imd_GetCondition(imd);
    ASSERT_EQ(true, condition.pwm_encoding.valid_duty_cycle);
    ASSERT_EQ(SST_BAD, condition.pwm_encoding.speed_start_status);

    get_pwm_duty_cycle_fake.return_val = BAD_MAX_DUTY_CYCLE + 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_pwm_encoding_for_device_error_condition)
{
    SetImdCondition(imd, IMD_DEVICE_ERROR, get_pwm_frequency_fake.return_val);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE - 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE;
    ASSERT_EQ(true, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE;
    ASSERT_EQ(true, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE + 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_pwm_encoding_for_earth_fault_condition)
{
    SetImdCondition(imd, IMD_EARTH_FAULT, get_pwm_frequency_fake.return_val);

    // From ISOMETER® IR155-3203/IR155-3204 manual:
    //     Duty cycle => 47.5...52.5%

    constexpr float MIN_VALID_DUTY_CYCLE = 47.5f;
    constexpr float MAX_VALID_DUTY_CYCLE = 52.5f;

    get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE - 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MIN_VALID_DUTY_CYCLE;
    ASSERT_EQ(true, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE;
    ASSERT_EQ(true, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);

    get_pwm_duty_cycle_fake.return_val = MAX_VALID_DUTY_CYCLE + 0.1f;
    ASSERT_EQ(false, App_Imd_GetCondition(imd).pwm_encoding.valid_duty_cycle);
}

TEST_F(ImdTest, check_mapping_for_frequency_to_condition)
{
    struct ConditionLut
    {
        float                  frequency;
        enum Imd_ConditionName condition_name;
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
        { 48.0f, IMD_EARTH_FAULT },
        { 49.0f, IMD_EARTH_FAULT },
        { 50.0f, IMD_EARTH_FAULT },
        { 51.0f, IMD_EARTH_FAULT },
        { 52.0f, IMD_EARTH_FAULT },
        { 53.0f, IMD_INVALID },
    };

    SetPwmFrequencyTolerance(imd, 2.0f);

    for (auto &entry : lookup_table)
    {
        get_pwm_frequency_fake.return_val = entry.frequency;
        ASSERT_EQ(App_Imd_GetCondition(imd).name, entry.condition_name);
    }
}
