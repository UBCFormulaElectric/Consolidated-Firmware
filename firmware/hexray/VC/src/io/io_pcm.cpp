#include "io_pcm.hpp"
#include "hw_gpios.hpp"
#include "main.h"

namespace io::pcm
{

void set(const bool enable)
{
    pcm_en.writePin(enable);
}
bool enabled()
{
    return pcm_en.readPin();
}

} // namespace io::pcm
