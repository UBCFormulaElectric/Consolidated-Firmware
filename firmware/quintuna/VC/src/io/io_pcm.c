#include "io_pcm.h"
#include "hw_gpios.h"

void io_pcm_enable(const bool enable)
{
    hw_gpio_writePin(&pgood, enable);
}