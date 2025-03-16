#include <stdio.h>
#include <stdint.h>

#include "hw_gpio.h"
#include "hw_gpios.h"

static int32_t position  = 0;  // current position count (can be positive or negative)
static uint8_t lastState = 0u; // last read 2-bit state from channel A and channel B

/*
 * Lookup table for state transitions.
 * The index is calculated by concatenating the previous state (MSB 2 bits)
 * and the current state (2 LSB bits). Eachh table entry gives the movement:
 *   +1 for a step in one direction
 *   -1 for the opposite
 *    0 for no change/inbalid.
 */
// clang-format off
static const int8_t encoder_table[16] = {
    0,  -1, 1,  0,      // prev 00: 00, 01, 10, 11
    1,  0,  0,  -1,     // prev 01: 00, 01, 10, 11
    -1, 0,  0,  1,    // prev 10: 00, 01, 10, 11
    0,  1,  -1, 0   // prev 11: 00, 01, 10, 11
};
// clang-format on

void rotaryEncoder_update(uint8_t channelA, uint8_t channelB)
{
    uint8_t state = (uint8_t)((channelA << 1) | channelB);
    uint8_t index = (uint8_t)((lastState << 2) | state);
    position += encoder_table[index];
    lastState = state;
}

int32_t rotaryEncoder_getPosition()
{
    return position;
}
