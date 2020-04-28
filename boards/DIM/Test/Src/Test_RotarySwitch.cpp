#include "Test_RotarySwitch.h"

DEFINE_FAKE_VALUE_FUNC(enum DriveMode, get_drive_mode);

void RotarySwitchTest::SetUp()
{
    rotary_switch = App_RotarySwitch_Create(get_drive_mode);
}

void RotarySwitchTest::TearDown()
{
    ASSERT_TRUE(rotary_switch != NULL);
    App_RotarySwitch_Destroy(rotary_switch);
    rotary_switch = NULL;
}

TEST_F(RotarySwitchTest, valid_drive_mode)
{
    enum DriveMode buffer = NUM_DRIVE_MODE;

    for (int mode = DRIVE_MODE1; mode != NUM_DRIVE_MODE; mode++)
    {
        get_drive_mode_fake.return_val = (enum DriveMode)mode;

        ASSERT_EQ(
            EXIT_CODE_OK,
            App_RotarySwitch_GetDriveMode(rotary_switch, &buffer));

        ASSERT_EQ(get_drive_mode_fake.return_val, buffer);
    }
}

TEST_F(RotarySwitchTest, invalid_drive_mode)
{
    enum DriveMode buffer = DRIVE_MODE1;

    get_drive_mode_fake.return_val = NUM_DRIVE_MODE;

    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RotarySwitch_GetDriveMode(rotary_switch, &buffer));

    ASSERT_EQ(get_drive_mode_fake.return_val, buffer);
}
