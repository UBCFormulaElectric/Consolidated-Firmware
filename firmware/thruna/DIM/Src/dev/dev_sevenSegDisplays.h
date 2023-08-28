#pragma once

#include <stdbool.h>
#include "hw_gpio.h"

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

typedef enum
{
    SEVEN_SEG_POSITION_BANK_L_DIGIT_L,
    SEVEN_SEG_POSITION_BANK_L_DIGIT_M,
    SEVEN_SEG_POSITION_BANK_L_DIGIT_R,
    SEVEN_SEG_POSITION_BANK_M_DIGIT_L,
    SEVEN_SEG_POSITION_BANK_M_DIGIT_M,
    SEVEN_SEG_POSITION_BANK_M_DIGIT_R,
    SEVEN_SEG_POSITION_BANK_R_DIGIT_L,
    SEVEN_SEG_POSITION_BANK_R_DIGIT_M,
    SEVEN_SEG_POSITION_BANK_R_DIGIT_R,
    NUM_SEVEN_SEGS,
} SevenSegPosition;

typedef struct {
    const Gpio* const srck_pin;
    const Gpio* const rck_pin;
    const Gpio* const ser_out_pin;
    const Gpio* const dimming_pin;
} SevenSegDigitsConfig;

/**
 * Initialize 7-segment digits.
 */
void dev_sevenSegDigits_init(const SevenSegDigitsConfig* config);

/**
 * Issue commands to the shift registers controlling the 7-segment displays.
 */
void dev_sevenSegDigits_writeCommands(void);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * specified 7-segment display during the next Io_SevenSegDisplays_WriteCommands().
 * @param position The position to write to.
 * @param digit The hexadecimal digit to display.
 * @param decimal_point Whether or not the decimal point should be turned on.
 */
void dev_sevenSegDigits_setValue(SevenSegPosition position, HexDigit digit, bool decimal_point);

/**
 * Disable the given hexadecimal digit.
 * @param position The position to write to.
 */
void dev_sevenSegDigits_disable(SevenSegPosition position);
