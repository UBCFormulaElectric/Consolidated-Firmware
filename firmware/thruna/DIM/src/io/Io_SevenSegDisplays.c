#include <stm32f4xx_hal.h>
#include <assert.h>

#include "App_SevenSegDisplays.h"
#include "App_SharedMacros.h"
#include "Io_SevenSegDisplays.h"
#include "main.h"

// LEDs are controlled by clocking commands into an 8-bit shift register (TPIC6C596PWR), which drives the 7-segs
#define SHIFT_REGISTER_SIZE 8
#define TEMP_NUM_DISPLAYS 9
#define MAX_HEX_VAL 20U

typedef struct
{
    uint16_t disable;
    uint16_t values[NUM_HEX_DIGITS * 2]; // two to account for the lookup values for the numbers with decimal points
} CommandLookupTable;

// clang-format off
static const CommandLookupTable command_lookup_table =
{
    .disable = 0x0,
    .values  =
    {
        0xFC, // 0x0
        0x60, // 0x1
        0xDA, // 0x2
        0xF2, // 0x3
        0x66, // 0x4
        0xB6, // 0x5
        0xBE, // 0x6
        0xE0, // 0x7
        0xFE, // 0x8
        0xE6, // 0x9
        0xFD, // 0x10
        0x61, // 0x11
        0xDB, // 0x12
        0xF3, // 0x13
        0x67, // 0x14
        0xB7, // 0x15
        0xBF, // 0x16
        0xE1, // 0x17
        0xFF, // 0x18
        0xE7, // 0x19
    }
};
// clang-format on

// The 7-segment displays are controlled by sending 8-bit command values to
// shift registers via SPI
static uint16_t commands[NUM_SEVEN_SEG_DISPLAYS];

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
    for (int display = 0; display < 9; display++)
    {
        const uint16_t display_data = commands[9 - display - 1]; // invert order

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

void Io_SevenSegDisplays_SetHexDigit(struct SevenSegHexDigit hex_digit, int DIGIT)
{
    if (!hex_digit.enabled)
    {
        commands[DIGIT] = command_lookup_table.disable;
    }
    else
    {
        assert(hex_digit.value < MAX_HEX_VAL);

        commands[DIGIT] = command_lookup_table.values[hex_digit.value];
    }
}
