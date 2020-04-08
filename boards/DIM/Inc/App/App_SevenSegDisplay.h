#pragma once

enum SevenSegDisplay_Digit
{
    SEVEN_SEG_DISPLAY_0,
    SEVEN_SEG_DISPLAY_1,
    SEVEN_SEG_DISPLAY_2,
    SEVEN_SEG_DISPLAY_3,
    SEVEN_SEG_DISPLAY_4,
    SEVEN_SEG_DISPLAY_5,
    SEVEN_SEG_DISPLAY_6,
    SEVEN_SEG_DISPLAY_7,
    SEVEN_SEG_DISPLAY_8,
    SEVEN_SEG_DISPLAY_9,
    NUM_SEVEN_SEG_DISPLAY_DIGITS,
};

enum SevenSegDisplay_Letter
{
    SEVEN_SEG_DISPLAY_A,
    SEVEN_SEG_DISPLAY_B,
    SEVEN_SEG_DISPLAY_C,
    SEVEN_SEG_DISPLAY_D,
    SEVEN_SEG_DISPLAY_E,
    SEVEN_SEG_DISPLAY_F,
    NUM_SEVEN_SEG_DISPLAY_LETTERS,
};

struct SevenSegDisplay *App_SevenSegDisplay_Create(
    void (*const set_digit)(enum SevenSegDisplay_Digit digit),
    void (*const set_letter)(enum SevenSegDisplay_Letter letter));

void App_SevenSegDisplay_Destroy(struct SevenSegDisplay *seven_seg_display);
void App_SevenSegDisplay_SetDigit(
    const struct SevenSegDisplay *seven_seg_display,
    enum SevenSegDisplay_Digit    digit);
void App_SevenSegDisplay_SetLetter(
    const struct SevenSegDisplay *seven_seg_display,
    enum SevenSegDisplay_Letter   letter);
