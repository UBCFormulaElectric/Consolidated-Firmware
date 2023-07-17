#include "led.h"
#include <stm32f4xx.h>

void led_init(Led *led, Gpio *output)
{
    led->output = output;
}

void led_enable(Led *led, bool on)
{
    gpio_write(led->output, on);
}