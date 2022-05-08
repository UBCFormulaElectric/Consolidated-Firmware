/**
 * @brief Shared library for wrapping HAL GPIO library
 */
#pragma once

#include <stm32f3xx_hal.h>

// clang-format on
struct GPIO_PinPort
{
    uint16_t      pin;
    GPIO_TypeDef *port;
};

/**
 * @brief Wrapper for HAL_GPIO_WritePin so we can use use-defined enums to
 *        replace GPIO_PinState without compiler warnings. This is useful
 *        because GPIO_PinState does not convey enough information for us. For
 *        example, writing `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)`
 *        does not convey if something is being turned on or off because PA0
 *        may use either active-high or active low logic.
 *
 *        Instead, we may define something like
 *
 *        typedef enum
 *        {
 *            FAN_ON = GPIO_PIN_SET,
 *            FAN_OFF = !FAN_ON;
 *        } FanOnOff_GPIO_PinState;
 *
 *        When we write `SharedGpio_GPIO_WritePin(GPIOA, GPIO_PIN_0, FAN_ON)`,
 *        it is now much more clear that a fan is being turned on.
 * @param GPIOx where x can be (A..H) to select the GPIO peripheral
 * @param GPIO_Pin specifies the port bit to be written.
 *        This parameter can be one of GPIO_PIN_x where x can be (0..15).
 * @param User_PinState Any user-define enum that replaces GPIO_PinState. Note
 *                      that uint32_t is used because it tolerates any enum
 *                      type without throwing any compiler warnings.
 * @param
 */

void Io_SharedGpio_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t User_PinState);
