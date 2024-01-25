#pragma once

struct RgbLedSequence;

/**
 * Allocate and initialize a RGB LED sequence
 * @param turn_on_red_led A function that can be called to turn the RGB LED red
 * @param turn_on_green_led A function that can be called to turn the RGB LED
 *                          green
 * @param turn_on_blue_led A function that can be called to turn the RGB LED
 *                         blue
 * @return The created RGB LED sequence, whose ownership is given to the caller
 */
struct RgbLedSequence *App_SharedRgbLedSequence_Create(
    void (*turn_on_red_led)(void),
    void (*turn_on_green_led)(void),
    void (*turn_on_blue_led)(void));

/**
 * Deallocate the memory used by the given RGB LED sequence
 * @param rgb_led_sequence The RGB LED sequence to deallocate
 */
void App_SharedRgbLedSequence_Destroy(struct RgbLedSequence *rgb_led_sequence);

/**
 * Update the given RGB LED sequence to turn on the next color
 * @param rgb_led_sequence The RGB LED sequence to update
 */
void App_SharedRgbLedSequence_Tick(struct RgbLedSequence *const rgb_led_sequence);
