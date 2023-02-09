#include <stm32f4xx_hal.h>
#include <assert.h>

#include "App_SevenSegDisplays.h"
#include "App_SharedMacros.h"
#include "Io_SevenSegDisplays.h"
#include "main.h"

// LEDs are controlled by clocking commands into an 8-bit shift register (TPIC6C596PWR), which drives the 7-segs
#define SHIFT_REGISTER_SIZE 8
#define TEMP_NUM_DISPLAYS 9

// Digits for displaying "Formula E"
#define DIGIT_F 0x8E
#define DIGIT_O 0x3A
#define DIGIT_R 0x0A
#define DIGIT_M1 0x2A
#define DIGIT_M2 0x2A
#define DIGIT_U 0x1C << 1
#define DIGIT_L 0x0E << 1
#define DIGIT_A 0x7D << 1
#define DIGIT_E 0x4F << 1

static uint8_t formula_e_commands[] = { DIGIT_F, DIGIT_O, DIGIT_R, DIGIT_M1, DIGIT_M2,
                                        DIGIT_U, DIGIT_L, DIGIT_A, DIGIT_E };

typedef struct
{
    uint8_t disable;
    uint8_t values[NUM_HEX_DIGITS];
} CommandLookupTable;

// clang-format off
static const CommandLookupTable command_lookup_table =
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

// The 7-segment displays are controlled by sending 8-bit command values to
// shift registers via SPI
static uint8_t commands[NUM_SEVEN_SEG_DISPLAYS];

void Io_SevenSegDisplays_Init()
{
    // RCK pin is normally held low
    HAL_GPIO_WritePin(SEVENSEGS_RCK_GPIO_Port, SEVENSEGS_RCK_Pin, GPIO_PIN_RESET);

    // Set 7-segs to full brightness
    HAL_GPIO_WritePin(SEVENSEGS_DIMMING_GPIO_Port, SEVENSEGS_DIMMING_Pin, GPIO_PIN_RESET);
}

void Io_SevenSegDisplays_WriteCommands(void)
{
    // The 7-segment displays are daisy chained by shifting registers, so we
    // can't update them individually. Instead, we must update the 7-segment
    // displays all at once.
    for (int display = 0; display < TEMP_NUM_DISPLAYS; display++)
    {
        // TODO: Update commands for 9 displays
        const uint8_t display_data = formula_e_commands[TEMP_NUM_DISPLAYS - display - 1]; // invert order

        for (int i = 0; i < SHIFT_REGISTER_SIZE; i++)
        {
            const bool bit_state = IS_BIT_SET(display_data, i);

            // Write bit state to data line
            HAL_GPIO_WritePin(
                SEVENSEGS_SEROUT_GPIO_Port, SEVENSEGS_SEROUT_Pin, bit_state ? GPIO_PIN_SET : GPIO_PIN_RESET);

            // Pulse clock to shift data in
            HAL_GPIO_WritePin(SEVENSEGS_SRCK_GPIO_Port, SEVENSEGS_SRCK_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEVENSEGS_SRCK_GPIO_Port, SEVENSEGS_SRCK_Pin, GPIO_PIN_RESET);
        }
    }

    // A pulse to RCK transfers data from the shift registers to the storage
    // registers, completing the write command.
    HAL_GPIO_WritePin(SEVENSEGS_RCK_GPIO_Port, SEVENSEGS_RCK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEVENSEGS_RCK_GPIO_Port, SEVENSEGS_RCK_Pin, GPIO_PIN_RESET);
}

void Io_SevenSegDisplays_SetLeftHexDigit(struct SevenSegHexDigit hex_digit)
{
    if (!hex_digit.enabled)
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
    if (!hex_digit.enabled)
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
    if (!hex_digit.enabled)
    {
        commands[RIGHT_SEVEN_SEG_DISPLAY] = command_lookup_table.disable;
    }
    else
    {
        assert(hex_digit.value < NUM_HEX_DIGITS);

        commands[RIGHT_SEVEN_SEG_DISPLAY] = command_lookup_table.values[hex_digit.value];
    }
}
