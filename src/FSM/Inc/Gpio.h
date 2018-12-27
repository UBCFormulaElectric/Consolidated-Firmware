/**
 * @file  Gpio.h
 * @brief GPIO helper functions library
 */
#ifndef GPIO_H
#define GPIO_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdbool.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Turn on the Red LED
 */
void Gpio_TurnOnRedLed(void);

/**
 * @brief Turn on the Blue LED
 */
void Gpio_TurnOnBlueLed(void);

/**
 * @brief Turn on the Green LED
 */
void Gpio_TurnOnGreenLed(void);

/**
 * @brief Check if the brake is pressed
 * @return true Brake is pressed
 *         false Brake is not pressed
 */
bool Gpio_IsBrakePressed(void);

/**
 * @brief Check is the ALARMD pin for PAPPS is indicating any error
 * @return true Active error
 *         false No active error
 */
bool Gpio_IsPappsAlarmdActive(void);

/**
 * @brief Check is the ALARMD pin for SAPPS is indicating any error
 * @return true Active error
 *         false No active error
 */
bool Gpio_IsSappsAlarmdActive(void);

#endif /* GPIO_H */
