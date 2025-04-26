#include "io_shift_register.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_pwmOutput.h"
#include "hw_pwms.h"

#define MAX_BRIGHTNESS 100.0f

/**
 * @brief Shift out one byte to the shift register (MSB-first).
 *
 * @param serin  Pointer to the GPIO used for serial data input.
 * @param srck   Pointer to the GPIO used for the shift register clock.
 * @param data   The 8-bit value to send, MSB first.
 */
static void shiftOutByte(const Gpio *serin, const Gpio *srck, uint8_t data)
{
    hw_gpio_writePin(srck, false);

    for (uint8_t bit = 0; bit < 8; bit++)
    {
        // Extract bit starting with the MSB.
        bool bitVal = (data & (1 << (7 - bit))) ? true : false;

        hw_gpio_writePin(serin, bitVal);
        hw_gpio_writePin(srck, true);
        hw_gpio_writePin(srck, false);
    }
}

/**
 * @brief Initialize LED shift register by enabling its PWM dimming.
 */
void io_shift_register_led_init(void)
{
    hw_pwmOutput_start(&led_dimming);
    hw_pwmOutput_setDutyCycle(&led_dimming, 1.0f);
}

/**
 * @brief Initialize seven-segment display to max brightness.
 */
void io_shift_register_seven_seg_init(void)
{
    hw_pwmOutput_start(&seven_seg_dimming);
    hw_pwmOutput_setDutyCycle(&seven_seg_dimming, MAX_BRIGHTNESS);
}

/**
 * @brief Update the LED shift register outputs.
 * Sends an array of bytes to the LED shift registers (MSB-first), then latches them.
 *
 * @param data  Array of bytes.
 */
void io_shift_register_updateLedRegisters(uint8_t *data)
{
    // Pull the LED register latch low to prepare shifting.
    hw_gpio_writePin(&led_rck, false);

    // Shift out each byte in reverse order so data[0] maps to the first register.
    for (int i = (LED_DATA_LENGTH - 1); i >= 0; i--)
    {
        shiftOutByte(&led_serin, &led_srck, data[i]);
    }

    // Pulse the latch line high then low to transfer shifted bits to outputs.
    hw_gpio_writePin(&led_rck, true);
    hw_gpio_writePin(&led_rck, false);
}

/**
 * @brief Update the seven-segment display shift register outputs.
 * Sends an array of bytes to the seven-segment shift registers (MSB-first), then latches them.
 *
 * @param data  Array of bytes of length SEVEN_SEG_DATA_LENGTH to shift out.
 */
void io_shift_register_updateSevenSegRegisters(uint8_t *data)
{
    // Pull the seven-seg latch low to prepare shifting
    hw_gpio_writePin(&seven_seg_rck, false);

    // Shift out each byte in reverse order so data[0] maps to the first register.
    for (int i = (SEVEN_SEG_DATA_LENGTH - 1); i >= 0; i--)
    {
        shiftOutByte(&seven_seg_serin, &seven_seg_srck, data[i]);
    }

    // Pulse the latch line high then low to update the segment outputs.
    hw_gpio_writePin(&seven_seg_rck, true);
    hw_gpio_writePin(&seven_seg_rck, false);
}

/**
 * @brief Set the dimming level of the LED shift register.
 * @param brightness_percent  Duty cycle for LED PWM (0.0f – 100.0f).
 */
void io_shift_register_led_setDimming(float brightness_percent)
{
    // Update the PWM duty cycle to control LED brightness
    hw_pwmOutput_setDutyCycle(&led_dimming, brightness_percent);
}
