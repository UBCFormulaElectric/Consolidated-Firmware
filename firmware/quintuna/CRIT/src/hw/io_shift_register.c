#include "io_shift_register.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_pwmOutput.h"
#include "hw_pwms.h"

#define MAX_BRIGHTNESS 100.0f

static void runDelay(void)
{
    for (uint8_t i = 0; i <= 30; i++)
    {
        __asm__("nop");
    }
}

// Shift out one byte to the shift register (MSB-first).
static void shiftOutByte(const Gpio *serin, const Gpio *srck, uint8_t data)
{
    hw_gpio_writePin(srck, false);
    runDelay();

    for (uint8_t bit = 0; bit < 8; bit++)
    {
        // Extract bit starting with the MSB.
        bool bitVal = (data & (1 << (7 - bit))) ? true : false;

        hw_gpio_writePin(serin, bitVal);
        runDelay();
        hw_gpio_writePin(srck, true);
        runDelay();
        hw_gpio_writePin(srck, false);
        runDelay();
    }
}

void io_shift_register_led_init(void)
{
    hw_pwmOutput_start(&led_dimming);
}

void io_shift_register_seven_seg_init(void)
{
    hw_pwmOutput_start(&seven_seg_dimming);
    hw_pwmOutput_setDutyCycle(&seven_seg_dimming, MAX_BRIGHTNESS);
}

void io_shift_register_updateLedRegisters(uint8_t *data)
{
    // Hold latch low.
    hw_gpio_writePin(&led_rck, false);

    // Shift each byte, starting with the last one
    for (int i = (LED_DATA_LENGTH - 1); i >= 0; i--)
    {
        shiftOutByte(&led_serin, &led_srck, data[i]);
    }

    // Latch the outputs
    hw_gpio_writePin(&led_rck, true);
    runDelay();
    hw_gpio_writePin(&led_rck, false);
}

void io_shift_register_updateSevenSegRegisters(uint8_t *data)
{
    // Hold latch low.
    hw_gpio_writePin(&seven_seg_rck, false);

    // Shift each byte, starting with the last one.
    for (int i = (SEVEN_SEG_DATA_LENGTH - 1); i >= 0; i--)
    {
        shiftOutByte(&seven_seg_serin, &seven_seg_srck, data[i]);
    }

    // Latch the outputs.
    hw_gpio_writePin(&seven_seg_rck, true);
    runDelay();
    hw_gpio_writePin(&seven_seg_rck, false);
}

void io_shift_register_led_setDimming(float brightness_percent)
{
    hw_pwmOutput_setDutyCycle(&led_dimming, brightness_percent);
}
