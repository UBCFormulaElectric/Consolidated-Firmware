#include "io_shift_register.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

// Shift out one byte to the shift register (MSB-first).
static void shiftOutByte(const Gpio *serin, const Gpio *srck, uint8_t data)
{
    for (uint8_t bit = 0; bit < 8; bit++)
    {
        // Extract bit starting with the MSB.
        bool bitVal = (data & (1 << (7 - bit))) ? true : false;

        hw_gpio_writePin(serin, bitVal);
        hw_gpio_writePin(srck, true);
        hw_gpio_writePin(srck, false);
    }
}

void io_shift_register_updateLedRegisters(uint8_t *data, uint8_t numBytes)
{
    // Hold latch low.
    hw_gpio_writePin(&led_rck, false);

    // Shift each byte, starting with the last one
    for (int i = (numBytes - 1); i >= 0; i--)
    {
        shiftOutByte(&led_serin, &led_srck, data[i]);
    }

    // Latch the outputs
    hw_gpio_writePin(&led_rck, true);
    // TODO: maybe slight delay needed 1ms???
    hw_gpio_writePin(&led_rck, false);
}

void io_shift_register_updateSevenSegRegisters(uint8_t *data, uint8_t numBytes)
{
    // Hold latch low.
    hw_gpio_writePin(&seven_seg_rck, false);

    // Shift each byte, starting with the last one.
    for (int i = (numBytes - 1); i >= 0; i--)
    {
        shiftOutByte(&seven_seg_serin, &seven_seg_srck, data[i]);
    }

    // Latch the outputs.
    hw_gpio_writePin(&seven_seg_rck, true);
    hw_gpio_writePin(&seven_seg_rck, false);
}

void io_shift_register_setDimming(const Gpio *dimming, bool state)
{
    hw_gpio_writePin(dimming, state);
}
