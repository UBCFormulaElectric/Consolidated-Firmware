#include "Test_Dcm.h"

extern "C"
{
#include "App_BrakeLight.h"
}

class BrakeLightTest : public testing::Test
{
  protected:
    void SetUp() override { App_BrakeLight_Init(); }
};

TEST_F(BrakeLightTest, non_actuated_brake_turns_off_brake_light)
{
    App_BrakeLight_SetLightStatus(false);
    ASSERT_EQ(App_BrakeLight_IsTurnedOn(), false);
}

TEST_F(BrakeLightTest, actuated_brake_turns_on_brake_light)
{
    App_BrakeLight_SetLightStatus(true);
    ASSERT_EQ(App_BrakeLight_IsTurnedOn(), true);
}
