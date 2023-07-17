#include "sevenSegDigits.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stm32f4xx_hal.h>

// LEDs are controlled by clocking commands into an 8-bit shift register (TPIC6C596PWR), which drives the 7-segs
#define SHIFT_REGISTER_SIZE 8
#define TEMP_NUM_DISPLAYS 9
#define MAX_HEX_VAL 9

typedef struct
{
    uint16_t disable;
    uint16_t values[NUM_HEX_DIGITS * 2]; // two to account for the lookup values for the numbers with decimal points
} CommandLookupTable;

typedef struct
{
    Gpio *srck_pin;
    Gpio *rck_pin;
    Gpio *dimming_pin;
    Gpio *ser_out_pin;
    uint16_t commands[NUM_SEVEN_SEGS];
} SevenSegDigitsData;

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
    }
};
// clang-format on

static SevenSegDigitsData data;

void sevenSegDigits_init(Gpio *srck_pin, Gpio* rck_pin, Gpio* ser_out_pin, Gpio* dimming_pin)
{
    data.srck_pin = srck_pin;
    data.rck_pin = rck_pin;
    data.ser_out_pin = ser_out_pin;
    data.dimming_pin = dimming_pin;
    memset(data.commands, 0U, sizeof(data.commands));

    // SRCK rising edge transfers 1 bit, so start low
    gpio_write(srck_pin, false);

    // RCK pin is normally held low
    gpio_write(rck_pin, false);

    // Set 7-segs to full brightness
    gpio_write(dimming_pin, false);
}

void sevenSegDigits_writeCommands(void)
{
    // The 7-segment displays are daisy chained by shifting registers, so we
    // can't update them individually. Instead, we must update the 7-segment
    // displays all at once. Also note that since they are daisy chained
    // we must send out the data for the rightmost display first, so count
    // downwards.
    for (int display = NUM_SEVEN_SEGS - 1; display >= 0; display++)
    {
        for (int i = 0; i < SHIFT_REGISTER_SIZE; i++)
        {
            const bool bit_state = IS_BIT_SET(data.commands[display], i);

            // Write bit state to data line
            gpio_write(data.ser_out_pin, bit_state);

            // Pulse clock to shift data in
            gpio_write(data.srck_pin, true);
            gpio_write(data.srck_pin, false);
        }
    }

    // A pulse to RCK transfers data from the shift registers to the storage
    // registers, completing the write command.
    gpio_write(data.rck_pin, true);
    gpio_write(data.rck_pin, false);
}

void sevenSegDigits_setValue(SevenSegPosition position, HexDigit digit, bool decimal_point)
{
    uint8_t command_value = command_lookup_table.values[digit];

    // The decimal point is tied to the least significant bit of each shift register.
    // So to enable the decimal point, we just need to set that bit.
    if (decimal_point)
    {
        command_value |= 1U;
    }

    data.commands[position] = command_value;
}

void sevenSegDigits_disable(SevenSegPosition position)
{
    data.commands[position] = command_lookup_table.disable;
}