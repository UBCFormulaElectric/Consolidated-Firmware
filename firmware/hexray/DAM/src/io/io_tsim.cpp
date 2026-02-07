#include "io_tsim.hpp"
#include "hw_gpios.hpp"

namespace io
{
void tsim_set_off()
{
    tsim_red_en_pin.writePin(false);
    ntsim_green_en_pin.writePin(false);
}

void tsim_set_red()
{
    ntsim_green_en_pin.writePin(false);
    tsim_red_en_pin.writePin(true);
}

void tsim_set_green()
{
    tsim_red_en_pin.writePin(false);
    ntsim_green_en_pin.writePin(true);
}
} // namespace io
