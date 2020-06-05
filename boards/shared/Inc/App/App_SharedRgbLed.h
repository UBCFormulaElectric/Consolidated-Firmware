#pragma once

struct RgbLed;

/**
 * Allocate and initialize a RGB LED
 * @param turn_red Turn the RGB LED red
 * @param turn_green Turn the RGB LED green
 * @param turn_blue Turn the RGB LED blue
 * @param turn_off Turn off the RGB LED
 * @return The created RGB LED, whose ownership is transferred to the caller
 */
struct RgbLed *App_SharedRgbLed_Create(
    void (*turn_red)(void),
    void (*turn_green)(void),
    void (*turn_blue)(void),
    void (*turn_off)(void));

/**
 * Deallocate the memory used by the given RGB LED
 * @param world The RGB LED to deallocate
 */
void App_SharedRgbLed_Destroy(struct RgbLed *rgb_led);

/**
 * Turn the given RGB LED red
 * @param rgb_led The RGB LED to turn red
 */
void App_SharedRgbLed_TurnRed(const struct RgbLed *rgb_led);

/**
 * Turn the given RGB LED green
 * @param rgb_led The RGB LED to turn green
 */
void App_SharedRgbLed_TurnGreen(const struct RgbLed *rgb_led);

/**
 * Turn the given RGB LED blue
 * @param rgb_led The RGB LED to turn blue
 */
void App_SharedRgbLed_TurnBlue(const struct RgbLed *rgb_led);

/**
 * Turn off the given RGB LED
 * @param rgb_led The RGB LED to turn off
 */
void App_SharedRgbLed_TurnOff(const struct RgbLed *rgb_led);
