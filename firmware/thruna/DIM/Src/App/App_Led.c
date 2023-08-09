#include <stdlib.h>
#include <assert.h>
#include "App_Led.h"

struct Led
{
    void (*turn_on_led)(void);
    void (*turn_off_led)(void);
};

struct Led *App_Led_Create(void (*turn_on_led)(void), void (*turn_off_led)(void))
{
    assert(turn_on_led != NULL);
    assert(turn_off_led != NULL);

    struct Led *led = malloc(sizeof(struct Led));
    assert(led != NULL);

    led->turn_on_led  = turn_on_led;
    led->turn_off_led = turn_off_led;

    return led;
}

void App_Led_Destroy(struct Led *led)
{
    free(led);
}

void App_Led_TurnOn(const struct Led *led)
{
    led->turn_on_led();
}

void App_Led_TurnOff(const struct Led *led)
{
    led->turn_off_led();
}
