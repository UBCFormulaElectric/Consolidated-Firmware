#include "test_BMSBase.hpp"
#include "app_imd.hpp"
#include <optional>

using app::can_utils::ImdActiveFrequency;
using app::can_utils::ImdConditionName;
using app::imd::Sst;
using enum app::can_utils::ImdActiveFrequency;
using enum app::can_utils::ImdConditionName;

class ImdTest : public BMSBaseTest
{
  public:
    static void SetImdCondition(const app::can_utils::ImdConditionName condition_name)
    {
        const std::map<ImdConditionName, float> mapping{
            { IMD_CONDITION_SHORT_CIRCUIT, 0.0f },          { IMD_CONDITION_NORMAL, 10.0f },
            { IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f }, { IMD_CONDITION_SST, 30.0f },
            { IMD_CONDITION_DEVICE_ERROR, 40.0f },          { IMD_CONDITION_GROUND_FAULT, 50.0f }
        };
        fakes::imd::setFrequency(mapping.at(condition_name));
        ASSERT_EQ(condition_name, app::imd::getCondition().name);
    }
};

TEST_F(ImdTest, check_insulation_resistance_normal_and_undervoltage_conditions)
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
        auto condition = app::imd::getCondition();
        ASSERT_EQ(false, condition.valid_duty_cycle);

        fakes::imd::setDutyCycle(MIN_VALID_DUTY_CYCLE);
        condition = app::imd::getCondition();
        ASSERT_EQ(true, condition.valid_duty_cycle);
        ASSERT_EQ(50000, condition.payload.insulation_measurement_dcp_kohms);

        fakes::imd::setDutyCycle((MIN_VALID_DUTY_CYCLE + MAX_VALID_DUTY_CYCLE) / 2.0f);
        condition = app::imd::getCondition();
        ASSERT_EQ(true, condition.valid_duty_cycle);
        ASSERT_EQ(1200, condition.payload.insulation_measurement_dcp_kohms);

        fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE + 0.01f);
        condition = app::imd::getCondition();
        ASSERT_EQ(false, condition.valid_duty_cycle);

        fakes::imd::setDutyCycle(MAX_VALID_DUTY_CYCLE);
        condition = app::imd::getCondition();
        ASSERT_EQ(true, condition.valid_duty_cycle);
        ASSERT_EQ(0, condition.payload.insulation_measurement_dcp_kohms);
    }
}

TEST_F(ImdTest, check_good_and_bad_sst_condition_eval)
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
    ASSERT_EQ(false, app::imd::getCondition().valid_duty_cycle);

    fakes::imd::setDutyCycle(GOOD_MIN_DUTY_CYCLE);
    auto condition = app::imd::getCondition();
    ASSERT_EQ(true, condition.valid_duty_cycle);
    ASSERT_EQ(Sst::GOOD, condition.payload.speed_start_status);

    fakes::imd::setDutyCycle(GOOD_MAX_DUTY_CYCLE);
    condition = app::imd::getCondition();
    ASSERT_EQ(true, condition.valid_duty_cycle);
    ASSERT_EQ(Sst::GOOD, condition.payload.speed_start_status);

    fakes::imd::setDutyCycle(GOOD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app::imd::getCondition().valid_duty_cycle);

    fakes::imd::setDutyCycle((GOOD_MIN_DUTY_CYCLE + BAD_MIN_DUTY_CYCLE) / 2.0f);
    ASSERT_EQ(false, app::imd::getCondition().valid_duty_cycle);

    fakes::imd::setDutyCycle(BAD_MIN_DUTY_CYCLE - 0.1f);
    ASSERT_EQ(false, app::imd::getCondition().valid_duty_cycle);

    fakes::imd::setDutyCycle(BAD_MIN_DUTY_CYCLE);
    condition = app::imd::getCondition();
    ASSERT_EQ(true, condition.valid_duty_cycle);
    ASSERT_EQ(Sst::BAD, condition.payload.speed_start_status);

    fakes::imd::setDutyCycle(BAD_MAX_DUTY_CYCLE);
    condition = app::imd::getCondition();
    ASSERT_EQ(true, condition.valid_duty_cycle);
    ASSERT_EQ(Sst::BAD, condition.payload.speed_start_status);

    fakes::imd::setDutyCycle(BAD_MAX_DUTY_CYCLE + 0.1f);
    ASSERT_EQ(false, app::imd::getCondition().valid_duty_cycle);
}

TEST_F(ImdTest, check_frequency_to_condition_mapping)
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
        { 7.0f, IMD_CONDITION_INVALID },
        { 8.0f, IMD_CONDITION_NORMAL },
        { 9.0f, IMD_CONDITION_NORMAL },
        { 10.0f, IMD_CONDITION_NORMAL },
        { 11.0f, IMD_CONDITION_NORMAL },
        { 12.0f, IMD_CONDITION_NORMAL },
        { 13.0f, IMD_CONDITION_INVALID },
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
        ASSERT_EQ(app::imd::getCondition().name, condition_name);
    }
}

TEST_F(ImdTest, check_can_broadcast_function)
{
    struct StatusLut
    {
        float                             frequency;
        std::optional<float>              duty_cycle;
        ImdConditionName                  condition_name;
        std::optional<ImdActiveFrequency> active_frequency;
    };
    const std::vector<StatusLut> lookup_table = {
        { 0.0f, std::nullopt, IMD_CONDITION_SHORT_CIRCUIT, IMD_0Hz },
        { 3.0f, std::nullopt, IMD_CONDITION_INVALID, std::nullopt },
        { 7.0f, std::nullopt, IMD_CONDITION_INVALID, std::nullopt },
        { 10.0f, 50.0f, IMD_CONDITION_NORMAL, IMD_10Hz },
        { 13.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 17.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 20.0f, 50.0f, IMD_CONDITION_UNDERVOLTAGE_DETECTED, IMD_20Hz },
        { 23.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 27.0f, 5.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 30.0f, 95.0f, IMD_CONDITION_SST, IMD_30Hz },
        { 33.0f, 5.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 37.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 40.0f, 50.0f, IMD_CONDITION_DEVICE_ERROR, IMD_40Hz },
        { 43.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 47.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
        { 50.0f, 50.0f, IMD_CONDITION_GROUND_FAULT, IMD_50Hz },
        { 53.0f, 50.0f, IMD_CONDITION_INVALID, std::nullopt },
    };

    for (const auto &[frequency, duty_cycle, condition_name, active_frequency_status] : lookup_table)
    {
        bool duty_cycle_set       = duty_cycle.has_value();
        bool duty_cycle_valid     = condition_name != IMD_CONDITION_INVALID;
        bool active_frequency_set = active_frequency_status.has_value();
        fakes::imd::setFrequency(frequency);
        if (duty_cycle_set)
        {
            fakes::imd::setDutyCycle(*duty_cycle);
        }

        app::imd::broadcast();

        ASSERT_EQ(app::imd::getCondition().name, condition_name);
        ASSERT_EQ(app::can_tx::BMS_ImdFrequency_get(), frequency);
        ASSERT_EQ(app::can_tx::BMS_ImdCondition_get(), condition_name);

        if (duty_cycle_set && duty_cycle_valid)
        {
            ASSERT_EQ(app::can_tx::BMS_ImdValidDutyCycle_get(), true);
            ASSERT_EQ(app::can_tx::BMS_ImdDutyCycle_get(), *duty_cycle);
        }
        ASSERT_EQ(active_frequency_set, app::can_tx::BMS_ImdCondition_get() != IMD_CONDITION_INVALID);

        if (active_frequency_set)
        {
            ASSERT_TRUE(app::can_tx::BMS_ImdActiveFrequency_get() == *active_frequency_status);
        }
    }
}
