#include "io_pcm.h"

void pcm_set(bool enable)
{
    hw::gpio::npcm_en.writePin(!enable);
}
