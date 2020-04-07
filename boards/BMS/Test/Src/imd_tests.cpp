#include "fff.h"
#include <gtest/gtest.h>
#include <utility>

extern "C"
{
#include "App_Imd.h"

    DEFINE_FFF_GLOBALS;

    FAKE_VALUE_FUNC(float, get_pwm_frequency);
    FAKE_VALUE_FUNC(float, get_pwm_frequency_tolerance);
    FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
    FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);
}

class Imd_Test : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        imd = App_Imd_Create(
            get_pwm_frequency, get_pwm_frequency_tolerance, get_pwm_duty_cycle,
            get_seconds_since_power_on);
    }
    virtual void TearDown() { App_Imd_Destroy(imd); }

    struct Imd *imd;
};

TEST_F(Imd_Test, check_condition_for_ideal_frequency)
{
    struct LookupTable
    {
        float              frequency;
        enum Imd_Condition condition;
    };

    std::vector<struct LookupTable> lookup_table = {
        { 0.0f, IMD_SHORT_CIRCUIT },
        { 1.0f, IMD_SHORT_CIRCUIT },
        { 2.0f, IMD_SHORT_CIRCUIT },
        { 3.0f, IMD_INVALID },
        { 4.0f, IMD_INVALID },
        { 5.0f, IMD_INVALID },
        { 6.0f, IMD_INVALID },
        { 7.0f, IMD_INVALID },
        { 8.0f, IMD_NORMAL },
        { 9.0f, IMD_NORMAL },
        { 10.0f, IMD_NORMAL },
        { 11.0f, IMD_NORMAL },
        { 12.0f, IMD_NORMAL },
        { 13.0f, IMD_INVALID },
        { 14.0f, IMD_INVALID },
        { 15.0f, IMD_INVALID },
        { 16.0f, IMD_INVALID },
        { 17.0f, IMD_INVALID },
        { 18.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 19.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 20.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 21.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 22.0f, IMD_UNDERVOLTAGE_DETECTED },
        { 23.0f, IMD_INVALID },
        { 24.0f, IMD_INVALID },
        { 25.0f, IMD_INVALID },
        { 26.0f, IMD_INVALID },
        { 27.0f, IMD_INVALID },
        { 28.0f, IMD_SST },
        { 29.0f, IMD_SST },
        { 30.0f, IMD_SST },
        { 31.0f, IMD_SST },
        { 32.0f, IMD_SST },
        { 33.0f, IMD_INVALID },
        { 34.0f, IMD_INVALID },
        { 35.0f, IMD_INVALID },
        { 36.0f, IMD_INVALID },
        { 37.0f, IMD_INVALID },
        { 38.0f, IMD_DEVICE_ERROR },
        { 39.0f, IMD_DEVICE_ERROR },
        { 40.0f, IMD_DEVICE_ERROR },
        { 41.0f, IMD_DEVICE_ERROR },
        { 42.0f, IMD_DEVICE_ERROR },
        { 43.0f, IMD_INVALID },
        { 44.0f, IMD_INVALID },
        { 45.0f, IMD_INVALID },
        { 46.0f, IMD_INVALID },
        { 47.0f, IMD_INVALID },
        { 48.0f, IMD_EARTH_FAULT },
        { 49.0f, IMD_EARTH_FAULT },
        { 50.0f, IMD_EARTH_FAULT },
        { 51.0f, IMD_EARTH_FAULT },
        { 52.0f, IMD_EARTH_FAULT },
        { 53.0f, IMD_INVALID },
        { 54.0f, IMD_INVALID },
        { 55.0f, IMD_INVALID },
    };

    get_pwm_frequency_tolerance_fake.return_val = 2.0f;

    for (auto &entry : lookup_table)
    {
        get_pwm_frequency_fake.return_val = entry.frequency;
        App_Imd_Tick(imd);
        ASSERT_EQ(App_Imd_GetCondition(imd), entry.condition);
    }
}
