#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio srck_gpio;
    const Gpio rck_gpio;
    const Gpio ser_out_gpio;
    const Gpio dimming_gpio;
} SevenSegsConfig;
#else
EMPTY_STRUCT(SevenSegsConfig);
#endif

// 7-segs are grouped into 3 groups of 3 displays.
typedef enum
{
    SEVEN_SEG_GROUP_L,
    SEVEN_SEG_GROUP_M,
    SEVEN_SEG_GROUP_R,
    NUM_SEVEN_SEG_GROUPS,
} SevenSegGroup;

// The position within each group.
typedef enum
{
    SEVEN_SEG_SUBPOSITION_L,
    SEVEN_SEG_SUBPOSITION_M,
    SEVEN_SEG_SUBPOSITION_R,
    NUM_SEVEN_SEG_SUBPOSITIONS,
} SevenSegSubposition;

typedef enum
{
    HEX_DIGIT_0,
    HEX_DIGIT_1,
    HEX_DIGIT_2,
    HEX_DIGIT_3,
    HEX_DIGIT_4,
    HEX_DIGIT_5,
    HEX_DIGIT_6,
    HEX_DIGIT_7,
    HEX_DIGIT_8,
    HEX_DIGIT_9,
    HEX_DIGIT_A,
    HEX_DIGIT_B,
    HEX_DIGIT_C,
    HEX_DIGIT_D,
    HEX_DIGIT_E,
    HEX_DIGIT_F,
    NUM_HEX_DIGITS,
} HexDigit;

/**
 * Initialize the 7-seg displays.
 */
void io_sevenSegDisplays_init(const SevenSegsConfig *seven_segs_config);

/**
 * Issue commands to the shift registers controlling the 7-segment displays.
 */
void io_sevenSegDisplays_writeCommands(void);

/**
 * Update the command value to send to the specified 7-segment display during the next sevenSegDisplays_writeCommands()
 * call.
 * @param group The group of the specified 7-seg.
 * @param subposition The position, within the group, of the specified 7-seg.
 * @param digit The hexadecimal value to display.
 * @param decimal_point Whether or not to enable the decimal point.
 */
void io_sevenSegDisplays_setValue(
    SevenSegGroup       group,
    SevenSegSubposition subposition,
    HexDigit            digit,
    bool                decimal_point);

/**
 * Disable the specified 7-segment display during the next sevenSegDisplays_writeCommands() call.
 * @param group The group of the specified 7-seg.
 * @param subposition The position, within the group, of the specified 7-seg.
 */
void io_sevenSegDisplays_disable(SevenSegGroup group, SevenSegSubposition subposition);
