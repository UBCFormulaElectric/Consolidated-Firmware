#include "io_rotary.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

#include "stm32f4xx_hal_exti.h"
#include <stdint.h>

// Global variables to track the encoder's state.
static volatile int32_t encoder_count = 0;
static volatile uint8_t last_encoded  = 0;

// Callback function pointers.
static volatile RotaryCallback clockwise_callback        = 0;
static volatile RotaryCallback counterclockwise_callback = 0;
static volatile RotaryCallback push_callback             = 0;

/**
 * @brief Update the encoder count and invoke the appropriate callback.
 *
 * Reads the current state of channels A and B, determines the rotation direction
 * using a 4-bit state (previous and current), and updates the count accordingly.
 * If a step is detected, the respective callback (clockwise or counter-clockwise)
 * is called if it has been set.
 */
static void update_encoder(void)
{
    // Read the current state of encoder channels.
    uint8_t msb     = hw_gpio_readPin(&rot_a);
    uint8_t lsb     = hw_gpio_readPin(&rot_b);
    uint8_t encoded = (uint8_t)(msb << 1) | lsb;

    // Combine previous state (last_encoded) and current state.
    uint8_t sum = (uint8_t)(last_encoded << 2) | encoded;

    // Patterns that indicate a clockwise step.
    if (sum == 0xD || sum == 0x4 || sum == 0x2 || sum == 0xB)
    {
        encoder_count++;
        if (clockwise_callback)
        {
            clockwise_callback();
        }
    }
    // Patterns that indicate a counter-clockwise step.
    else if (sum == 0xE || sum == 0x7 || sum == 0x1 || sum == 0x8)
    {
        encoder_count--;
        if (counterclockwise_callback)
        {
            counterclockwise_callback();
        }
    }

    // Save the current state for the next update.
    last_encoded = encoded;
}

/**
 * @brief Initialize the rotary encoder driver.
 *
 * Configures the encoder channels and push button pins as inputs, sets up external
 * interrupts for both rotation channels and the push button, and initializes the last state.
 */
void io_rotary_init(void)
{
    // Initialize the last state based on current readings.
    uint8_t msb  = hw_gpio_readPin(&rot_a);
    uint8_t lsb  = hw_gpio_readPin(&rot_b);
    last_encoded = (uint8_t)(msb << 1) | lsb;
}

/**
 * @brief Get the current position count.
 *
 * @return int32_t The current encoder count.
 */
int32_t io_rotary_get_position(void)
{
    return encoder_count;
}

/**
 * @brief Reset the encoder count to zero.
 */
void io_rotary_reset(void)
{
    encoder_count = 0;
}

/**
 * @brief Set the callback for a clockwise rotation.
 *
 * @param cb Function pointer to the callback.
 */
void io_rotary_setClockwiseCallback(RotaryCallback cb)
{
    clockwise_callback = cb;
}

/**
 * @brief Set the callback for a counter-clockwise rotation.
 *
 * @param cb Function pointer to the callback.
 */
void io_rotary_setCounterClockwiseCallback(RotaryCallback cb)
{
    counterclockwise_callback = cb;
}

/**
 * @brief Set the callback for the push button press.
 *
 * @param cb Function pointer to the callback.
 */
void io_rotary_setPushCallback(RotaryCallback cb)
{
    push_callback = cb;
}

/**
 * @brief Interrupt handler for encoder channel A.
 *
 * Clears the interrupt flag and calls the update function.
 */
void io_roatary_rotA_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_a.pin);
    update_encoder();
}

/**
 * @brief Interrupt handler for encoder channel B.
 *
 * Clears the interrupt flag and calls the update function.
 */
void io_rotary_rotB_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_b.pin);
    update_encoder();
}

/**
 * @brief Interrupt handler for the push button.
 *
 * Clears the interrupt flag and calls the push callback if it is set.
 */
void io_rotary_push_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_s.pin);
    push_callback();
}
