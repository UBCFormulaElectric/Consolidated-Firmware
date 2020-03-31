#include "fff.h"
#include <gtest/gtest.h>
#include <utility>

extern "C"
{
#include "App_Imd.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);

class Imd_Test : public testing::Test
{
  protected:
    void SetUp() override
    {
        imd = App_Imd_Create(
            get_pwm_frequency, get_pwm_duty_cycle, get_seconds_since_power_on);
    }

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
        { 0.0, IMD_SHORT_CIRCUIT },
        { 1.0, IMD_SHORT_CIRCUIT },
        { 2.0, IMD_SHORT_CIRCUIT },
        { 3.0, IMD_INVALID },
        { 4.0, IMD_INVALID },
        { 5.0, IMD_INVALID },
        { 6.0, IMD_INVALID },
        { 7.0, IMD_INVALID },
        { 8.0, IMD_NORMAL },
        { 9.0, IMD_NORMAL },
        { 10.0, IMD_NORMAL },
        { 11.0, IMD_NORMAL },
        { 12.0, IMD_NORMAL },
        { 13.0, IMD_INVALID },
        { 14.0, IMD_INVALID },
        { 15.0, IMD_INVALID },
        { 16.0, IMD_INVALID },
        { 17.0, IMD_INVALID },
        { 18.0, IMD_UNDERVOLTAGE_DETECTED },
        { 19.0, IMD_UNDERVOLTAGE_DETECTED },
        { 20.0, IMD_UNDERVOLTAGE_DETECTED },
        { 21.0, IMD_UNDERVOLTAGE_DETECTED },
        { 22.0, IMD_UNDERVOLTAGE_DETECTED },
        { 23.0, IMD_INVALID },
        { 24.0, IMD_INVALID },
        { 25.0, IMD_INVALID },
        { 26.0, IMD_INVALID },
        { 27.0, IMD_INVALID },
        { 28.0, IMD_SST },
        { 29.0, IMD_SST },
        { 30.0, IMD_SST },
        { 31.0, IMD_SST },
        { 32.0, IMD_SST },
        { 33.0, IMD_INVALID },
        { 34.0, IMD_INVALID },
        { 35.0, IMD_INVALID },
        { 36.0, IMD_INVALID },
        { 37.0, IMD_INVALID },
        { 38.0, IMD_DEVICE_ERROR },
        { 39.0, IMD_DEVICE_ERROR },
        { 40.0, IMD_DEVICE_ERROR },
        { 41.0, IMD_DEVICE_ERROR },
        { 42.0, IMD_DEVICE_ERROR },
        { 43.0, IMD_INVALID },
        { 44.0, IMD_INVALID },
        { 45.0, IMD_INVALID },
        { 46.0, IMD_INVALID },
        { 47.0, IMD_INVALID },
        { 48.0, IMD_EARTH_FAULT },
        { 49.0, IMD_EARTH_FAULT },
        { 50.0, IMD_EARTH_FAULT },
        { 51.0, IMD_EARTH_FAULT },
        { 52.0, IMD_EARTH_FAULT },
        { 53.0, IMD_INVALID },
        { 54.0, IMD_INVALID },
        { 55.0, IMD_INVALID },
    };

    for (auto &entry : lookup_table)
    {
        get_pwm_frequency_fake.return_val = entry.frequency;
        App_Imd_Tick(imd);
        ASSERT_EQ(App_Imd_GetCondition(imd), entry.condition);
    }
}
