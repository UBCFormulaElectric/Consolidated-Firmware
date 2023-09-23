#include <assert.h>
#include <stdlib.h>
#include "App_SharedRgbLed.h"

struct RgbLed
{
    void (*turn_red)(void);
    void (*turn_green)(void);
    void (*turn_blue)(void);
    void (*turn_off)(void);
};

struct RgbLed *App_SharedRgbLed_Create(
    void (*turn_red)(void),
    void (*turn_green)(void),
    void (*turn_blue)(void),
    void (*turn_off)(void))
{
    struct RgbLed *rgb_led = malloc(sizeof(struct RgbLed));
    assert(rgb_led != NULL);

    rgb_led->turn_red   = turn_red;
    rgb_led->turn_green = turn_green;
    rgb_led->turn_blue  = turn_blue;
    rgb_led->turn_off   = turn_off;

    return rgb_led;
}

void App_SharedRgbLed_Destroy(struct RgbLed *rgb_led)
{
    free(rgb_led);
}

void App_SharedRgbLed_TurnRed(const struct RgbLed *rgb_led)
{
    rgb_led->turn_red();
}

void App_SharedRgbLed_TurnGreen(const struct RgbLed *rgb_led)
{
    rgb_led->turn_green();
}

void App_SharedRgbLed_TurnBlue(const struct RgbLed *rgb_led)
{
    rgb_led->turn_blue();
}

void App_SharedRgbLed_TurnOff(const struct RgbLed *rgb_led)
{
    rgb_led->turn_off();
}
