#include "Test_RotarySwitch.h"

DEFINE_FAKE_VALUE_FUNC(uint32_t, get_position);

void RotarySwitchTest::SetUp()
{
    rotary_switch =
        App_RotarySwitch_Create(get_position, NUM_ROTARY_SWITCH_POSITIONS);
}

void RotarySwitchTest::TearDown()
{
    ASSERT_TRUE(rotary_switch != NULL);
    App_RotarySwitch_Destroy(rotary_switch);
    rotary_switch = NULL;
}

TEST_F(RotarySwitchTest, valid_switch_position)
{
    uint32_t buffer = NUM_ROTARY_SWITCH_POSITIONS;

    for (size_t pos = 0; pos != NUM_ROTARY_SWITCH_POSITIONS; pos++)
    {
        get_position_fake.return_val = (uint32_t)pos;

        ASSERT_EQ(
            EXIT_CODE_OK, App_RotarySwitch_GetPosition(rotary_switch, &buffer));
        ASSERT_EQ(get_position_fake.return_val, buffer);
    }
}

TEST_F(RotarySwitchTest, invalid_switch_position)
{
    uint32_t buffer = 0;

    get_position_fake.return_val = NUM_ROTARY_SWITCH_POSITIONS;

    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RotarySwitch_GetPosition(rotary_switch, &buffer));
    ASSERT_EQ(get_position_fake.return_val, buffer);
}
