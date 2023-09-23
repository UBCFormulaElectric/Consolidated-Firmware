#include "Test_Shared.h"

extern "C"
{
#include "App_SharedRgbLedSequence.h"
}

FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

class RgbLedSequenceTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
    }

    void TearDown() override { TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy); }

    struct RgbLedSequence *rgb_led_sequence;
};

TEST_F(RgbLedSequenceTest, cycle_red_green_blue)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count, &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    // Verify that we cycle through red, green, blue, red, etc. for 100 times
    for (size_t i = 0; i < 100; i++)
    {
        App_SharedRgbLedSequence_Tick(rgb_led_sequence);
        ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
    }
}
