#pragma once

/**
 * Allocate and initialize a LED
 * @param turn_on_led A function that can be called to turn on the LED
 * @param turn_off_led A function that can be called to turn off the LED
 * @return The created LED, whose ownership is given to the caller
 */
struct Led *App_Led_Create(void (*turn_on_led)(void), void (*turn_off_led)(void));

/**
 * Deallocate the memory used by the given LED
 * @param led The LED to deallocate
 */
void App_Led_Destroy(struct Led *led);

/**
 * Turn on the given LED
 * @param led The LED to turn on
 */
void App_Led_TurnOn(const struct Led *led);

/**
 * Turn off the given LED
 * @param led The LED to turn off
 */
void App_Led_TurnOff(const struct Led *led);
