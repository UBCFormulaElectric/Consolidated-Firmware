#include <gtest/gtest.h>
#include "Test_Utils.h"

extern "C"
{
#include "app_avgPower.h"
}

class AveragePowerCalcTest : public testing::Test
{
  protected:
    void SetUp() override { app_avgPower_init(); }
};

TEST_F(AveragePowerCalcTest, check_avg_power_calculations)
{
    app_avgPower_enable(true);
    float test_val = app_avgPower_update(100.3);
    float avg      = 100.3f;
    ASSERT_EQ(test_val, avg);

    test_val = app_avgPower_update(2.330);
    avg      = (100.3f + 2.330) / 2;
    ASSERT_EQ(test_val, avg);

    test_val = app_avgPower_update(90.9);
    avg      = (100.3f + 2.330 + 90.9) / 3;
    ASSERT_EQ(test_val, avg);
}
