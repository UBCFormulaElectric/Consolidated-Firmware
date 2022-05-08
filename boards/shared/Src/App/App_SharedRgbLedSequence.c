#include <stdlib.h>
#include <assert.h>
#include "App_SharedRgbLedSequence.h"

enum RgbLedSequenceState
{
    RED_LED_ON,
    GREEN_LED_ON,
    BLUE_LED_ON,
};

struct RgbLedSequence
{
    void (*turn_on_red_led)();
    void (*turn_on_green_led)();
    void (*turn_on_blue_led)();
    enum RgbLedSequenceState state;
};

struct RgbLedSequence *App_SharedRgbLedSequence_Create(
    void (*const turn_on_red_led)(void),
    void (*const turn_on_green_led)(void),
    void (*const turn_on_blue_led)(void))
{
    assert(turn_on_red_led != NULL);
    assert(turn_on_green_led != NULL);
    assert(turn_on_blue_led != NULL);

    struct RgbLedSequence *rgb_led_sequence = malloc(sizeof(struct RgbLedSequence));

    assert(rgb_led_sequence != NULL);

    rgb_led_sequence->turn_on_red_led   = turn_on_red_led;
    rgb_led_sequence->turn_on_green_led = turn_on_green_led;
    rgb_led_sequence->turn_on_blue_led  = turn_on_blue_led;

    // Arbitrary choice to start in the RED_LED_ON state
    rgb_led_sequence->state = RED_LED_ON;

    return rgb_led_sequence;
}

void App_SharedRgbLedSequence_Destroy(struct RgbLedSequence *const rgb_led_sequence)
{
    free(rgb_led_sequence);
}

void App_SharedRgbLedSequence_Tick(struct RgbLedSequence *const rgb_led_sequence)
{
    // This state machine is so simple that there's no need to use the shared
    // state machine library.
    switch (rgb_led_sequence->state)
    {
        case RED_LED_ON:
        {
            rgb_led_sequence->turn_on_red_led();
            rgb_led_sequence->state = GREEN_LED_ON;
            break;
        }
        case GREEN_LED_ON:
        {
            rgb_led_sequence->turn_on_green_led();
            rgb_led_sequence->state = BLUE_LED_ON;
            break;
        }
        case BLUE_LED_ON:
        {
            rgb_led_sequence->turn_on_blue_led();
            rgb_led_sequence->state = RED_LED_ON;
            break;
        }
    }
}
