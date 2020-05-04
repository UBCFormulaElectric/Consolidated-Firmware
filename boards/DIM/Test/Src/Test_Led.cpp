#include "Test_Led.h"

extern "C"
{
#include "App_Led.h"
}

FAKE_VOID_FUNC(turn_on_led);
FAKE_VOID_FUNC(turn_off_led);

void LedTest::TearDownLed(struct Led *&led_to_teardown)
{
    ASSERT_TRUE(led_to_teardown != NULL);
    App_Led_Destroy(led_to_teardown);
    led_to_teardown = NULL;
}

void LedTest::SetUp()
{
    led = App_Led_Create(turn_on_led, turn_off_led);

    RESET_FAKE(turn_on_led);
    RESET_FAKE(turn_off_led);
}

void LedTest::TearDown()
{
    ASSERT_TRUE(led != NULL);
    App_Led_Destroy(led);
    led = NULL;
}

TEST_F(LedTest, toggle_led)
{
    App_Led_TurnOff(led);
    ASSERT_EQ(0, turn_on_led_fake.call_count);
    ASSERT_EQ(1, turn_off_led_fake.call_count);

    App_Led_TurnOn(led);
    ASSERT_EQ(1, turn_on_led_fake.call_count);
    ASSERT_EQ(1, turn_off_led_fake.call_count);

    App_Led_TurnOff(led);
    ASSERT_EQ(1, turn_on_led_fake.call_count);
    ASSERT_EQ(2, turn_off_led_fake.call_count);
}
