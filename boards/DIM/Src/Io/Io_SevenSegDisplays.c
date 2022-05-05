#include <stm32f3xx_hal.h>
#include <assert.h>

#include "App_SevenSegDisplays.h"
#include "Io_SevenSegDisplays.h"
#include "main.h"

static_assert(HEX_DIGIT_0 == 0, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_1 == 1, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_2 == 2, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_3 == 3, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_4 == 4, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_5 == 5, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_6 == 6, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_7 == 7, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_8 == 8, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_9 == 9, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_A == 10, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_B == 11, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_C == 12, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_D == 13, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_E == 14, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_F == 15, "Hex enum must match its numeric value.");

struct CommandLookupTable
{
    uint8_t disable;
    uint8_t values[NUM_HEX_DIGITS];
};

static SPI_HandleTypeDef *_hspi;

// The 7-segment displays are controlled by sending 8-bit command values to
// shift registers via SPI
static uint8_t commands[NUM_SEVEN_SEG_DISPLAYS];

// clang-format off
static const struct CommandLookupTable command_lookup_table =
{
    .disable = 0x0,
    .values  =
    {
        0x3F, // 0x0
        0x06, // 0x1
        0x5B, // 0x2
        0x4F, // 0x3
        0x66, // 0x4
        0x6D, // 0x5
        0x7D, // 0x6
        0x07, // 0x7
        0x7F, // 0x8
        0x67, // 0x9
        0x77, // 0xA
        0x7C, // 0xB
        0x39, // 0xC
        0x5E, // 0xD
        0x79, // 0xE
        0x71, // 0xF
    }
};
// clang-format on

void Io_SevenSegDisplays_Init(SPI_HandleTypeDef *const hspi)
{
    _hspi = hspi;

    // RCK pin is normally held low
    HAL_GPIO_WritePin(SEVENSEG_RCK_3V3_GPIO_Port, SEVENSEG_RCK_3V3_Pin, GPIO_PIN_RESET);

    // Full brightness
    HAL_GPIO_WritePin(SEVENSEG_DIMMING_3V3_GPIO_Port, SEVENSEG_DIMMING_3V3_Pin, GPIO_PIN_RESET);
}

void Io_SevenSegDisplays_WriteCommands(void)
{
    // The 7-segment displays are daisy chained by shifting registers, so we
    // can't update them individually. Instead, we must update the 7-segment
    // displays all at once.
    HAL_SPI_Transmit(_hspi, commands, NUM_SEVEN_SEG_DISPLAYS, 100U);

    // A pulse to RCK transfers data from the shift registers to the storage
    // registers, completing the write command.
    HAL_GPIO_TogglePin(SEVENSEG_RCK_3V3_GPIO_Port, SEVENSEG_RCK_3V3_Pin);
    HAL_GPIO_TogglePin(SEVENSEG_RCK_3V3_GPIO_Port, SEVENSEG_RCK_3V3_Pin);
}

void Io_SevenSegDisplays_SetLeftHexDigit(struct SevenSegHexDigit hex_digit)
{
    if (hex_digit.enabled == false)
    {
        commands[LEFT_SEVEN_SEG_DISPLAY] = command_lookup_table.disable;
    }
    else
    {
        assert(hex_digit.value < NUM_HEX_DIGITS);

        commands[LEFT_SEVEN_SEG_DISPLAY] = command_lookup_table.values[hex_digit.value];
    }
}

void Io_SevenSegDisplays_SetMiddleHexDigit(struct SevenSegHexDigit hex_digit)
{
    if (hex_digit.enabled == false)
    {
        commands[MIDDLE_SEVEN_SEG_DISPLAY] = command_lookup_table.disable;
    }
    else
    {
        assert(hex_digit.value < NUM_HEX_DIGITS);

        commands[MIDDLE_SEVEN_SEG_DISPLAY] = command_lookup_table.values[hex_digit.value];
    }
}

void Io_SevenSegDisplays_SetRightHexDigit(struct SevenSegHexDigit hex_digit)
{
    if (hex_digit.enabled == false)
    {
        commands[RIGHT_SEVEN_SEG_DISPLAY] = command_lookup_table.disable;
    }
    else
    {
        assert(hex_digit.value < NUM_HEX_DIGITS);

        commands[RIGHT_SEVEN_SEG_DISPLAY] = command_lookup_table.values[hex_digit.value];
    }
}
