#include "Test_Fsm.h"

extern "C"
{
#include "App_WheelSpeedSensor.h"
}

FAKE_VALUE_FUNC(float, get_wheel_speed);
FAKE_VALUE_FUNC(enum WheelSpeedStatus, get_wheel_speed_status);

class WheelSpeedSensorTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        wheel_speed_sensor = App_WheelSpeedSensor_Create(get_wheel_speed);
        RESET_FAKE(get_wheel_speed);
    }

    void TearDown() override
    {
        ASSERT_TRUE(wheel_speed_sensor != NULL);
        App_WheelSpeedSensor_Destroy(wheel_speed_sensor);
        wheel_speed_sensor = NULL;
    }

    struct WheelSpeedSensor *wheel_speed_sensor;
};

TEST_F(WheelSpeedSensorTest, check_if_wheel_speed_is_updated_after_tick)
{
    get_wheel_speed_fake.return_val = 1.0f;
    ASSERT_EQ(
        get_wheel_speed_fake.return_val,
        App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor));
}

TEST_F(WheelSpeedSensorTest, check_if_wheel_speed_non_critical_fault_is_updated)
{
    constexpr float MAX_WHEEL_SPEED = 150.0;

    get_wheel_speed_fake.return_val = MAX_WHEEL_SPEED - 0.01f;
    App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor);
    ASSERT_EQ(
        WHEEL_SPEED_NORMAL, App_WheelSpeedSensor_GetStatus(wheel_speed_sensor));

    get_wheel_speed_fake.return_val = MAX_WHEEL_SPEED;
    App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor);
    ASSERT_EQ(
        WHEEL_SPEED_NORMAL, App_WheelSpeedSensor_GetStatus(wheel_speed_sensor));

    get_wheel_speed_fake.return_val = MAX_WHEEL_SPEED + 0.01f;
    App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor);
    ASSERT_EQ(
        WHEEL_SPEED_NON_CRITICAL_FAULT,
        App_WheelSpeedSensor_GetStatus(wheel_speed_sensor));

    // 0x400000 is a NaN; Check that wheel speed sensor non critical fault is
    // active
    get_wheel_speed_fake.return_val = 0x400000;
    App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor);
    ASSERT_EQ(
        WHEEL_SPEED_NON_CRITICAL_FAULT,
        App_WheelSpeedSensor_GetStatus(wheel_speed_sensor));
}
