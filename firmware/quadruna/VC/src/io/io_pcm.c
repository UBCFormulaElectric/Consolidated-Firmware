#include "io_pcm.h"
#include "hw_gpios.h"

void io_pcm_set(const bool enable)
{
    hw_gpio_writePin(&npcm_en, !enable);
}
