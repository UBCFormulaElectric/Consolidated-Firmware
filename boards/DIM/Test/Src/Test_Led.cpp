#include "Test_Led.h"

extern "C"
{
#include "App_Led.h"
}

FAKE_VOID_FUNC(prv_turn_on_led);
FAKE_VOID_FUNC(prv_turn_off_led);

void LedTest::SetupLed(
    struct Led *&led,
    void (*turn_on_led)(void),
    void (*turn_off_led)(void))
{
    led = App_Led_Create(turn_on_led, turn_off_led);
}

void LedTest::TearDownLed(struct Led *&led)
{
    ASSERT_TRUE(led != NULL);
    App_Led_Destroy(led);
    led = NULL;
}

void LedTest::SetUp()
{
    prv_led = App_Led_Create(prv_turn_on_led, prv_turn_off_led);
}

void LedTest::TearDown()
{
    ASSERT_TRUE(prv_led != NULL);
    App_Led_Destroy(prv_led);
    prv_led = NULL;
}
