#include "io_pcm.hpp"
#include "hw_gpios.hpp"
#include "main.h"

void io::pcm::set(const bool enable)
{
    pcm_en.writePin(enable);
}
bool io::pcm::enabled()
{
    return pcm_en.readPin();
}
