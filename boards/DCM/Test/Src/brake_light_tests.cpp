#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_BrakeLight.h"
}

TEST_F(BrakeLightTest, non_actuated_brake_and_inactive_regen_turns_off_brake_light)
{
    is_break_actuated_fake.return_val = false;
    is_regen_active_fake.return_val = false;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_OFF);
}

TEST_F(BrakeLightTest, non_actuated_brake_and_active_regen_turns_on_brake_light)
{
    is_break_actuated_fake.return_val = false;
    is_regen_active_fake.return_val = true;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}

TEST_F(BrakeLightTest, actuated_brake_and_inactive_regen_turns_on_brake_light)
{
    is_break_actuated_fake.return_val = true;
    is_regen_active_fake.return_val = false;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}

TEST_F(BrakeLightTest, actuated_brake_and_active_regen_turns_on_brake_light)
{
    is_break_actuated_fake.return_val = true;
    is_regen_active_fake.return_val = true;

    App_BrakeLight_Tick(brake_light);
    ASSERT_EQ(App_BrakeLight_GetStatus(brake_light), BRAKE_LIGHT_ON);
}