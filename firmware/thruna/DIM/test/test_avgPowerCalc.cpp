#include "test_dimBaseStateMachine.h"

class AveragePowerCalcTest : public DimBaseStateMachineTest
{
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

TEST_F(AveragePowerCalcTest, avg_power_calc_resets_with_switch)
{
    app_avgPower_enable(true);
    float test_val = app_avgPower_update(45.6);
    float avg      = 45.6f;
    ASSERT_EQ(test_val, avg);

    app_avgPower_enable(false);
    test_val = app_avgPower_update(78.9);
    ASSERT_EQ(test_val, 0);
}
