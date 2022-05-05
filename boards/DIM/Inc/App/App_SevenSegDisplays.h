#pragma once

#include <stdint.h>
#include "App_SharedExitCode.h"

struct SevenSegDisplay;

enum
{
    LEFT_SEVEN_SEG_DISPLAY,
    MIDDLE_SEVEN_SEG_DISPLAY,
    RIGHT_SEVEN_SEG_DISPLAY,
    NUM_SEVEN_SEG_DISPLAYS,
};

/**
 * Allocate and initialize a group of three 7-segment displays
 * @param left_seven_seg_display The left 7-segment display
 * @param middle_seven_seg_display The middle 7-segment display
 * @param right_seven_seg_display The right 7-segment display
 * @param display_value_callback The function to call after we display a value
 *                                on the 7-segment displays
 * @note This function does __not__ take ownership of any of the 7-segment
 *       displays passed into it, which means the every interface must be kept
 *       alive for the lifetime of this created group of 7-segment displays
 * @return A pointer to the created group of 7-segment displays, whose
 *         ownership is given to the caller
 */
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *left_seven_seg_display,
    struct SevenSegDisplay *middle_seven_seg_display,
    struct SevenSegDisplay *right_seven_seg_display,
    void (*display_value_callback)(void));

/**
 * Deallocate the memory used by the given group of 7-segment displays
 * @param seven_seg_displays The group of 7-segment displays to deallocate
 */
void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

/**
 * Display hexadecimal digits on the given group of 7-segment displays
 * @param seven_seg_displays The group of 7-segment displays to display
 *                           hexadecimal digits on
 * @param hex_digits The hexadecimal digits to display
 * @param num_hex_digits The number of hexadecimal digits to display, up to a
 *                       maximum of 3
 * @return EXIT_CODE_INVALID_ARGS if more than 3 hexadecimal digits are
 *         requested, or if any of the requested digits contains a value that
 *         is not in the range of [0x0-0xF]
 */
ExitCode App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *seven_seg_displays,
    const uint8_t                  hex_digits[],
    size_t                         num_hex_digits);

/**
 * Display an unsigned base-10 value on the given group of 7-segment displays
 * @note The value that can be displayed is constrained by the number of
 *       7-segment displays. Use the return code to see if the requested value
 *       is valid.
 * @param seven_seg_displays The group of 7-segment displays to display base-10
 *                           value on
 * @param value The unsigned base-10 value to display
 * @return EXIT_CODE_INVALID_ARGS if the given value is out-of-bound
 */
ExitCode App_SevenSegDisplays_SetUnsignedBase10Value(const struct SevenSegDisplays *seven_seg_displays, uint32_t value);
