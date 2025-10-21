#include "io/shift_register.h"

#include "hw/gpios.h"
#include "hw/pwmOutput.h"
#include "hw/pwms.h"

#define MAX_BRIGHTNESS 100.0f

/**
 * @brief Shift out one byte to the shift register (MSB first).
 *
 * @param serin  Pointer to the GPIO used for serial data input.
 * @param srck   Pointer to the GPIO used for the shift register clock.
 * @param data   The 8 bit value to send, MSB first.
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
 * @brief Initialize LED shift reg by enabling its PWM dimmin.
 */
void io_shift_register_led_init(void)
{
    hw_pwmOutput_start(&led_dimming);
    hw_pwmOutput_setDutyCycle(&led_dimming, 1.0f);
}

/**
 * @brief Initialize seven seg display to max brightness.
 */
void io_shift_register_seven_seg_init(void)
{
    hw_pwmOutput_start(&seven_seg_dimming);
    hw_pwmOutput_setDutyCycle(&seven_seg_dimming, MAX_BRIGHTNESS);
}

/**
 * @brief Update the led shift register outputs.
 * Sends an array of bytes to the led shift registers (MSB-first), then latches them.
 *
 * @param data  Array of bytes.
 */
void io_shift_register_updateLedRegisters(uint8_t *data)
{
    // Pull the led register latch low to prepare shifting.
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
 * @brief Update the seven seg display shift register outputs.
 * Sends an array of bytes to the seven seg shift registers (MSB-first), then latches them.
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
 * @brief Set the dimming level of the led shift register.
 * @param brightness_percent  Duty cycle for led pwm (0.0f to 100.0f).
 */
void io_shift_register_led_setDimming(float brightness_percent)
{
    hw_pwmOutput_setDutyCycle(&led_dimming, brightness_percent);
}
