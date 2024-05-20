#include "test_rsmBaseStateMachineTest.h"
#include <cmath>

class RsmAppTest : public RsmBaseStateMachineTest
{
  protected:
    template <typename T>
    void CheckInRangeCanSignals(
        float min_val,
        float max_val,
        void (*fake_setter)(float),
        T (*can_signal_val_getter)(),
        bool (*alert_getter)())

    {
        // Testing for values in the normal range
        const float nominal_val = (min_val + max_val) / 2;
        fake_setter(nominal_val);
        LetTimePass(10);
        ASSERT_EQ(nominal_val, can_signal_val_getter());
        ASSERT_FALSE(alert_getter());

        // Underflow range
        fake_setter(std::nextafter(min_val, std::numeric_limits<float>::lowest()));
        LetTimePass(10);
        ASSERT_TRUE(alert_getter());

        // Overflow range
        fake_setter(std::nextafter(max_val, std::numeric_limits<float>::max()));
        LetTimePass(10);
        ASSERT_TRUE(alert_getter());
    }

    void CheckBinaryStatusCanSignal(void (*fake_setter)(bool), bool (*can_signal_getter)())
    {
        fake_setter(true);
        LetTimePass(10);
        ASSERT_TRUE(can_signal_getter());

        fake_setter(false);
        LetTimePass(10);
        ASSERT_FALSE(can_signal_getter());
    }
};

// check flowrate signal in range
TEST_F(RsmAppTest, check_flowrate_can_signals)
{
    CheckInRangeCanSignals(
        MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN, fake_io_coolant_getFlowRate_returns,
        app_canTx_RSM_CoolantFlowRate_get, app_canTx_RSM_Warning_FlowRateOutOfRange_get);
}

TEST_F(RsmAppTest, check_temperatuereA_can_signals)
{
    fake_io_coolant_getTemperatureA_returns(21.2);
    LetTimePass(10);
    ASSERT_NEAR(app_canTx_RSM_CoolantTemperatureA_get(), 21.2, 0.0001);
}

TEST_F(RsmAppTest, check_temperatuereB_can_signals)
{
    fake_io_coolant_getTemperatureB_returns(50.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_CoolantTemperatureB_get(), 50.0);
}

// Transfer functio is not written yet, just testing voltage value
TEST_F(RsmAppTest, check_pressureA_can_signals)
{
    fake_io_coolant_getPressureA_returns(0.1);
    LetTimePass(10);
    ASSERT_NEAR(app_canTx_RSM_CoolantPressureA_get(), 0.1, 0.0001);
}

// Transfer functio is not written yet, just testing voltage value
TEST_F(RsmAppTest, check_pressureB_can_signals)
{
    fake_io_coolant_getPressureB_returns(0.8);
    LetTimePass(10);
    ASSERT_NEAR(app_canTx_RSM_CoolantPressureB_get(), 0.8, 0.00001);
}

TEST_F(RsmAppTest, check_left_rear_suspension_can_signals)
{
    fake_io_suspension_getRearLeftTravel_returns(50.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_RearLeftSuspensionTravel_get(), 50.0);

    CheckBinaryStatusCanSignal(
        fake_io_suspension_leftSensorOCSC_returns, app_canAlerts_RSM_Warning_LeftSuspensionOCSC_get);
}

TEST_F(RsmAppTest, check_right_rear_suspension_can_signals)
{
    fake_io_suspension_getRearRightTravel_returns(50.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_RearRightSuspensionTravel_get(), 50.0);

    CheckBinaryStatusCanSignal(
        fake_io_suspension_rightSensorOCSC_returns, app_canAlerts_RSM_Warning_RightSuspensionOCSC_get);
}

TEST_F(RsmAppTest, check_fan_can_signals)
{
    fake_io_acc_fan_set_returnsForAnyArgs(1);
    fake_io_rad_fan_set_returnsForAnyArgs(1);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_AccumulatorFan_get(), 1);
    ASSERT_EQ(app_canTx_RSM_RadiatorFan_get(), 1);

    fake_io_acc_fan_set_returnsForAnyArgs(0);
    fake_io_rad_fan_set_returnsForAnyArgs(0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_AccumulatorFan_get(), 0);
    ASSERT_EQ(app_canTx_RSM_RadiatorFan_get(), 0);
}

TEST_F(RsmAppTest, check_brakelight_can_signals)
{
    fake_io_brake_light_set_returnsForAnyArgs(0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_BrakeLight_get(), 0);

    fake_io_brake_light_set_returnsForAnyArgs(1);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_RSM_BrakeLight_get(), 1);
}
