#include "io_rotary.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

#include "stm32f4xx_hal_exti.h"
#include <pb.h>
#include <stdint.h>

// Callback function pointers.
static volatile RotaryCallback clockwise_callback        = 0;
static volatile RotaryCallback counterclockwise_callback = 0;
static volatile RotaryCallback push_callback             = 0;

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
void io_rotary_rotA_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_a.pin);

    if (hw_gpio_readPin(&rot_b))
    {
        if (counterclockwise_callback != NULL)
        {
            counterclockwise_callback();
        }
    }
    else
    {
        if (clockwise_callback != NULL)
        {
            clockwise_callback();
        }
    }
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
