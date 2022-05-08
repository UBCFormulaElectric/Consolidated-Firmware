#include "Test_Dcm.h"

extern "C"
{
#include "App_BrakeLight.h"
}

FAKE_VOID_FUNC(turn_on_brake_light);
FAKE_VOID_FUNC(turn_off_brake_light);

class BrakeLightTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        brake_light = App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);

        FFF_RESET_HISTORY();
    }
    void TearDown() override { TearDownObject(brake_light, App_BrakeLight_Destroy); }

    struct BrakeLight *brake_light;
};

TEST_F(BrakeLightTest, non_actuated_brake_and_inactive_regen_turns_off_brake_light)
{
    App_BrakeLight_SetLightStatus(brake_light, false, false);

    ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), false);
    ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);
}

TEST_F(BrakeLightTest, non_actuated_brake_and_active_regen_turns_on_brake_light)
{
    App_BrakeLight_SetLightStatus(brake_light, false, true);

    ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
    ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
}

TEST_F(BrakeLightTest, actuated_brake_and_inactive_regen_turns_on_brake_light)
{
    App_BrakeLight_SetLightStatus(brake_light, true, false);

    ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
    ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
}

TEST_F(BrakeLightTest, actuated_brake_and_active_regen_turns_on_brake_light)
{
    App_BrakeLight_SetLightStatus(brake_light, true, true);

    ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
    ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
}
