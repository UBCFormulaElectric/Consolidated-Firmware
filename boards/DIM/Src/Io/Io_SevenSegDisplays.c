#include <stm32f3xx_hal.h>

#include "App_SharedAssert.h"
#include "App_SharedConstants.h"

#include "Io_SevenSegDisplays.h"

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

struct SevenSegDisplay_Commands
{
    uint8_t left;
    uint8_t middle;
    uint8_t right;
};

static SPI_HandleTypeDef _hspi;

static struct SevenSegDisplay_Commands commands;

static const uint8_t command_lookup_table[NUM_HEX_DIGITS] = {
    0x03, // 0x0
    0x4F, // 0x1
    0x25, // 0x2
    0x0D, // 0x3
    0x99, // 0x4
    0x49, // 0x5
    0x41, // 0x6
    0x1F, // 0x7
    0x01, // 0x8
    0x09, // 0x9
    0x11, // 0xA
    0x1C, // 0xB
    0x63, // 0xC
    0x85, // 0xD
    0x61, // 0xE
    0x71, // 0xF
};


void Io_SevenSegDisplays_Init(SPI_HandleTypeDef hspi)
{
    _hspi = hspi;
}

void Io_SevenSegDisplays_WriteCommands(void)
{
    // The 7-segment displays are daisy chained by shifting registers, so we
    // can't update them individually. Instead, we must update the 7-segment
    // displays all at once.
    HAL_SPI_Transmit(&_hspi, (uint8_t*)&commands, 3U, 100U);
}

float Io_SevenSegDisplays_GetStateOfCharge(void)
{
    // TODO: Read the state of charge from CAN
    return 0.0f;
}

void Io_SevenSegDisplays_SetLeftHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    commands.left = command_lookup_table[hex_digit];
}

void Io_SevenSegDisplays_SetMiddleHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    commands.middle = command_lookup_table[hex_digit];
}

void Io_SevenSegDisplays_SetRightHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    commands.right = command_lookup_table[hex_digit];
}
