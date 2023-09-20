#include "Test_Shared.h"

extern "C"
{
#include "io_led.h"
}

#include "fake_io_led.hpp"

// TODO: This test is pretty much useless, and only here to demonstrate using 
// fakegen. Delete this once fakegen is validated on other systems.

class TestLed : public testing::Test
{
  protected:
    void SetUp() override
    {  
        fake_io_led_enable_reset();
    }

    BinaryLed led = {};
};

TEST_F(TestLed, toggle_led)
{
    io_led_enable(&led, false);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&led, false));

    io_led_enable(&led, true);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&led, false));

    io_led_enable(&led, false);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&led, false));
}
