#include "io/pcm.h"
#include "hw/gpios.h"
#include "main.h"

void io_pcm_set(const bool enable)
{
    hw_gpio_writePin(&pcm_en, enable);
}

bool io_pcm_enabled()
{
    return hw_gpio_readPin(&pcm_en);
}

void io_pcm_toggle()
{
    hw_gpio_togglePin(&pcm_en);
}
