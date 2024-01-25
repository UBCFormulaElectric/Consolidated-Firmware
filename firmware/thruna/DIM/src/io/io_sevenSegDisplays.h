#pragma once

#include "App_SharedConstants.h"
#include "App_SharedMacros.h"
#include <stdbool.h>

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

/**
 * Initialize the 7-seg displays.
 */
void io_sevenSegDisplays_init(const SevenSegsConfig* seven_segs_config);

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
