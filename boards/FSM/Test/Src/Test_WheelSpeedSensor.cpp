#include "Test_Fsm.h"

extern "C"
{
#include "App_CanTx.h"
#include "App_Wheel.h"
}

FAKE_VALUE_FUNC(float, get_wheel_speed);

class WheelSpeedSensorTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        wheel_speed_sensor = App_Wheel_Create(get_wheel_speed, 150.0f);
        RESET_FAKE(get_wheel_speed);
    }

    void TearDown() override
    {
        ASSERT_TRUE(wheel_speed_sensor != NULL);
        App_Wheel_Destroy(wheel_speed_sensor);
        wheel_speed_sensor = NULL;
    }

    struct WheelSpeedSensor *wheel_speed_sensor;
};

TEST_F(WheelSpeedSensorTest, check_if_wheel_speed_is_updated)
{
    get_wheel_speed_fake.return_val = 1.0f;
    ASSERT_EQ(
        get_wheel_speed_fake.return_val,
        App_Wheel_GetWheelSpeed(wheel_speed_sensor));
}
