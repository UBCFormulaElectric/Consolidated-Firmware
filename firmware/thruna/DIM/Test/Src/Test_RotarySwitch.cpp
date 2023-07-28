#include "Test_Dim.h"

extern "C"
{
#include "App_RotarySwitch.h"
}

FAKE_VALUE_FUNC(uint32_t, get_position);

class RotarySwitchTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        rotary_switch = App_RotarySwitch_Create(get_position, DEFAULT_NUM_ROTARY_SWITCH_POSITIONS);
    }

    void TearDown() override { TearDownObject(rotary_switch, App_RotarySwitch_Destroy); }

    struct RotarySwitch *rotary_switch;
    const uint32_t       DEFAULT_NUM_ROTARY_SWITCH_POSITIONS = 6;
};

TEST_F(RotarySwitchTest, valid_switch_position)
{
    uint32_t buffer = DEFAULT_NUM_ROTARY_SWITCH_POSITIONS;

    for (size_t pos = 0; pos != DEFAULT_NUM_ROTARY_SWITCH_POSITIONS; pos++)
    {
        get_position_fake.return_val = (uint32_t)pos;

        ASSERT_EQ(EXIT_CODE_OK, App_RotarySwitch_GetSwitchPosition(rotary_switch, &buffer));
        ASSERT_EQ(get_position_fake.return_val, buffer);
    }
}

TEST_F(RotarySwitchTest, invalid_switch_position)
{
    uint32_t buffer = 0;

    get_position_fake.return_val = DEFAULT_NUM_ROTARY_SWITCH_POSITIONS;

    ASSERT_EQ(EXIT_CODE_OUT_OF_RANGE, App_RotarySwitch_GetSwitchPosition(rotary_switch, &buffer));
    ASSERT_EQ(get_position_fake.return_val, buffer);
}
