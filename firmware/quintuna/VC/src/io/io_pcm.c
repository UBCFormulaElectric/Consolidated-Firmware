#include "io_pcm.h"
#include "hw_gpios.h"
#include "main.h"

void io_pcm_set(const bool enable)
{
    hw_gpio_writePin(pcm_en, enable);
}

void io_pcm_get()
{
    hw_gpio_readPin(pcm_en);
}

void io_pcm_toggle()
{
    hw_gpio_togglePin(pcm_en);
}
