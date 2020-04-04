#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_BrakeLight.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VALUE_FUNC(bool, is_brake_actuated);
    FAKE_VALUE_FUNC(bool, is_regen_active);
    FAKE_VOID_FUNC(turn_on_brake_light);
    FAKE_VOID_FUNC(turn_off_brake_light);
}

class BrakeLightTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        RESET_FAKE(is_brake_actuated);
        RESET_FAKE(is_regen_active);

        FFF_RESET_HISTORY();

        brake_light = App_BrakeLight_Create(
            is_brake_actuated, is_regen_active, turn_on_brake_light,
            turn_off_brake_light);
    }
    virtual void TearDown() { App_BrakeLight_Destroy(brake_light); }

    struct BrakeLight *brake_light;
};

TEST_F(
    BrakeLightTest,
    non_actuated_brake_and_inactive_regen_turns_off_brake_light)
{
    is_brake_actuated_fake.return_val = false;
    is_regen_active_fake.return_val   = false;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_OFF);
}

TEST_F(BrakeLightTest, non_actuated_brake_and_active_regen_turns_on_brake_light)
{
    is_brake_actuated_fake.return_val = false;
    is_regen_active_fake.return_val   = true;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}

TEST_F(BrakeLightTest, actuated_brake_and_inactive_regen_turns_on_brake_light)
{
    is_brake_actuated_fake.return_val = true;
    is_regen_active_fake.return_val   = false;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}

TEST_F(BrakeLightTest, actuated_brake_and_active_regen_turns_on_brake_light)
{
    is_brake_actuated_fake.return_val = true;
    is_regen_active_fake.return_val   = true;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}