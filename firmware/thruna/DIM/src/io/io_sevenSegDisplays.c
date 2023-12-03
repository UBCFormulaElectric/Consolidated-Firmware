#include "io_sevenSegDisplays.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stm32f4xx_hal.h>

// LEDs are controlled by clocking commands into an 8-bit shift register (TPIC6C596PWR), which drives the 7-segs.
#define SHIFT_REGISTER_SIZE 8
#define NUM_SEVEN_SEGS (NUM_SEVEN_SEG_GROUPS * NUM_SEVEN_SEG_SUBPOSITIONS)

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

static const CommandLookupTable faultCodes = { .disable = 0x0,
                                               .values  = {
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
                                               } };

static const SevenSegsConfig *config;
static uint8_t                commands[NUM_SEVEN_SEGS];

void io_sevenSegDisplays_init(const SevenSegsConfig *seven_segs_config)
{
    config = seven_segs_config;
    memset(commands, 0U, sizeof(commands));

    // SRCK rising edge transfers 1 bit, so start low.
    hw_gpio_writePin(&config->srck_gpio, false);

    // RCK pin is normally held low.
    hw_gpio_writePin(&config->rck_gpio, false);

    // Set 7-segs to full brightness.
    hw_gpio_writePin(&config->dimming_gpio, false);
}

void io_sevenSegDisplays_writeCommands(void)
{
    // The 7-segment displays are daisy-chained by shifting registers, so we
    // can't update them individually. Instead, we must update the 7-segment
    // displays all at once. Also note that since they are daisy-chained
    // we must send out the data for the rightmost display first, so count
    // downwards.
    for (int display = NUM_SEVEN_SEGS - 1; display >= 0; display--)
    {
        for (int i = 0; i < SHIFT_REGISTER_SIZE; i++)
        {
            const bool bit_state = commands[display] & (1 << i);

            // Write bit state to data line
            hw_gpio_writePin(&config->ser_out_gpio, bit_state);

            // Pulse clock to shift data in
            hw_gpio_writePin(&config->srck_gpio, true);
            hw_gpio_writePin(&config->srck_gpio, false);
        }
    }

    // A pulse to RCK transfers data from the shift registers to the storage
    // registers, completing the write command.
    hw_gpio_writePin(&config->rck_gpio, true);
    hw_gpio_writePin(&config->rck_gpio, false);
}

void io_sevenSegDisplays_setValue(
    SevenSegGroup       group,
    SevenSegSubposition subposition,
    HexDigit            digit,
    bool                decimal_point)
{
    int     index         = NUM_SEVEN_SEG_SUBPOSITIONS * group + subposition;
    uint8_t command_value = command_lookup_table.values[digit];

    // The decimal point is tied to the least significant bit of each shift register.
    // So to enable the decimal point, we just need to set that bit.
    if (decimal_point)
    {
        command_value |= 1U;
    }

    commands[index] = command_value;
}

void io_sevenSegDisplays_disable(SevenSegGroup group, SevenSegSubposition subposition)
{
    int index       = NUM_SEVEN_SEG_SUBPOSITIONS * group + subposition;
    commands[index] = command_lookup_table.disable;
}