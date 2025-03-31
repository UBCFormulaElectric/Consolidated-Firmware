#include "hw_gpio.h"
#include "hw_gpios.h"

#include "io_rotary.h"
#include "io_time.h"
#include "io_time.h"

#define DEBOUNCE_TIME_MS 50

static int32_t encoder_position = 0;
static uint8_t prev_state       = 0; // Holds the previous state of channel A and B

/* Global variables for debouncing the switch */
static uint32_t lastSwitchTime  = 0;
static uint8_t  lastSwitchState = 1; // Internal pull-up so unpressed state is high

/*
 * Lookup table for state transitions.
 * The index is calculated by concatenating the previous state (MSB 2 bits)
 * and the current state (2 LSB bits). Eachh table entry gives the movement:
 *   +1 for a step in one direction
 *   -1 for the opposite
 *    0 for no change/inbalid.
 */
static const int8_t encoder_table[16] = {
    0,  -1, 1,  0,  // prev 00: 00, 01, 10, 11
    1,  0,  0,  -1, // prev 01: 00, 01, 10, 11
    -1, 0,  0,  1,  // prev 10: 00, 01, 10, 11
    0,  1,  -1, 0   // prev 11: 00, 01, 10, 11
};

static uint8_t read_channels(void)
{
    uint8_t state = 0u;

    if (hw_gpio_readPin(&rot_a) == true)
    {
        state |= 0x01;
    }
    if (hw_gpio_readPin(&rot_b) == true)
    {
        state |= 0x02;
    }

    return state;
}

void io_rotary_init(void)
{
    prev_state = read_channels();
}

uint32_t io_rotary_toggleEventHandler(void)
{
    uint8_t curr_state = read_channels();

    // Combine previous and current state to build a 4 bit index
    uint8_t index = (uint8_t)(prev_state << 2) | curr_state;
    int8_t  delta = encoder_table[index];

    encoder_position += delta;
    prev_state = curr_state;

    return (uint32_t)encoder_position;
}

uint8_t io_rotary_pressEventHandler(void)
{
    uint32_t currentTime  = io_time_getCurrentMs();
    uint8_t  currentState = hw_gpio_readPin(&rot_s);

    // Check if switch has changed
    if (currentState != lastSwitchState)
    {
        // Wait for debounce
        if ((currentTime - lastSwitchTime) > DEBOUNCE_TIME_MS)
        {
            lastSwitchState = currentState;
            lastSwitchTime  = currentTime;
        }
    }
    else
    {
        // If state is unchanged, refresh the debounce timer.
        lastSwitchTime = currentTime;
    }

    return lastSwitchState;
}
