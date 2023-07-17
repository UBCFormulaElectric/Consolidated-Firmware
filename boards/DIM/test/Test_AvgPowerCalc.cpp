#include "Test_Dim.h"

extern "C"
{
#include "App_AvgPower.h"
}

class AveragePowerCalcTest : public testing::Test
{
  protected:
    void SetUp() override { avg_power_calc = App_AvgPowerCalc_Create(); }

    void TearDown() override { TearDownObject(avg_power_calc, App_AvgPowerCalc_Destroy); }

    struct AvgPowerCalc *avg_power_calc;
};

TEST_F(AveragePowerCalcTest, check_avg_power_calculations)
{
    App_AvgPowerCalc_Enable(avg_power_calc, true);
    float test_val = App_AvgPowerCalc_Update(avg_power_calc, 100.3);
    float avg      = 100.3f;
    ASSERT_EQ(test_val, avg);

    test_val = App_AvgPowerCalc_Update(avg_power_calc, 2.330);
    avg      = (100.3f + 2.330) / 2;
    ASSERT_EQ(test_val, avg);

    test_val = App_AvgPowerCalc_Update(avg_power_calc, 90.9);
    avg      = (100.3f + 2.330 + 90.9) / 3;
    ASSERT_EQ(test_val, avg);
}
