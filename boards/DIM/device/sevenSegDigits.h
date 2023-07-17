#pragma once

#include <stdbool.h>
#include "gpio.h"

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

void sevenSegDigits_init(Gpio *srck_pin, Gpio *rck_pin, Gpio *ser_out_pin, Gpio *dimming_pin);
void sevenSegDigits_writeCommands(void);
void sevenSegDigits_setValue(SevenSegPosition position, HexDigit digit, bool decimal_point);
void sevenSegDigits_disable(SevenSegPosition position);
