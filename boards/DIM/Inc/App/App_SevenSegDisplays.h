#pragma once

#include <stdint.h>
#include <stddef.h>
#include "App_SharedExitCode.h"

#define NUM_IN_GROUP 3

struct SevenSegDisplay;

enum
{
    LEFT_L_SEVEN_SEG_DISPLAY,
    LEFT_M_SEVEN_SEG_DISPLAY,
    LEFT_R_SEVEN_SEG_DISPLAY,
    MIDDLE_L_SEVEN_SEG_DISPLAY,
    MIDDLE_M_SEVEN_SEG_DISPLAY,
    MIDDLE_R_SEVEN_SEG_DISPLAY,
    RIGHT_L_SEVEN_SEG_DISPLAY,
    RIGHT_M_SEVEN_SEG_DISPLAY,
    RIGHT_R_SEVEN_SEG_DISPLAY,
    NUM_SEVEN_SEG_DISPLAYS,
};

/**
 * Allocate and initialize a group of three 7-segment displays
 * @param left_l_seven_seg_display The leftmost of the left trio 7-segment display
 * @param left_m_seven_seg_display The middlemost of the left trio 7-segment display
 * @param left_r_seven_seg_display The rightmost of the left trio 7-segment display
 * @param middle_l_seven_seg_display The leftmost of the middle trio 7-segment display
 * @param middle_m_seven_seg_display The middlemost of the middle trio 7-segment display
 * @param middle_r_seven_seg_display The rightmost of the middle trio 7-segment display
 * @param right_l_seven_seg_display The leftmost of the right trio 7-segment display
 * @param right_m_seven_seg_display The middlemost of the right trio 7-segment display
 * @param right_r_seven_seg_display The rightmost of the right trio 7-segment display
 * @param display_value_callback The function to call after we display a value
 *                                on the 7-segment displays
 * @note This function does __not__ take ownership of any of the 7-segment
 *       displays passed into it, which means the every interface must be kept
 *       alive for the lifetime of this created group of 7-segment displays
 * @return A pointer to the created group of 7-segment displays, whose
 *         ownership is given to the caller
 */
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *left_l_seven_seg_display,
    struct SevenSegDisplay *left_m_seven_seg_display,
    struct SevenSegDisplay *left_r_seven_seg_display,
    struct SevenSegDisplay *middle_l_seven_seg_display,
    struct SevenSegDisplay *middle_m_seven_seg_display,
    struct SevenSegDisplay *middle_r_seven_seg_display,
    struct SevenSegDisplay *right_l_seven_seg_display,
    struct SevenSegDisplay *right_m_seven_seg_display,
    struct SevenSegDisplay *right_r_seven_seg_display,
    void (*display_value_callback)(void));

/**
 * Deallocate the memory used by the given group of 7-segment displays
 * @param seven_seg_displays The group of 7-segment displays to deallocate
 */
void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

/**
 * Populate the digits array with the for the given index of 3 for the given group.
 *
 * @param seven_seg_displays The group of 7-segment displays to display that are currently
 *                           being worked on.
 * @param digits[] The array of the full 9 7-segment displays values
 * @param index The index to indicate which
 */
void App_SevenSegDisplays_SetDigits(const struct SevenSegDisplays *seven_seg_displays, uint8_t index);

/**
 * Take in a base 10 value and populate it to the left most group of 7-segment displays
 *
 * @param seven_seg_displays The group of 7-segment displays to display that are currently
 *                           being worked on.
 * @param digits[] The array of the full 9 7-segment displays values to populate
 * @param value The base 10 value to be written to the specified group of 7-segments
 */
ExitCode App_SevenSegDisplays_SetGroupL(const struct SevenSegDisplays *const seven_seg_displays, float value);

/**
 * Take in a base 10 value and populate it to the middle group of 7-segment displays
 *
 * @param seven_seg_displays The group of 7-segment displays to display that are currently
 *                           being worked on.
 * @param digits[] The array of the full 9 7-segment displays values to populate
 * @param value The base 10 value to be written to the specified group of 7-segments
 */
ExitCode App_SevenSegDisplays_SetGroupM(const struct SevenSegDisplays *const seven_seg_displays, float value);

/**
 * Take in a base 10 value and populate it to the right most group of 7-segment displays
 *
 * @param seven_seg_displays The group of 7-segment displays to display that are currently
 *                           being worked on.
 * @param digits[] The array of the full 9 7-segment displays values to populate
 * @param value The base 10 value to be written to the specified group of 7-segments
 */
ExitCode App_SevenSegDisplays_SetGroupR(const struct SevenSegDisplays *const seven_seg_displays, float value);
