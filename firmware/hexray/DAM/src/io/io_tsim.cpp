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
    tsim_red_en_pin.writePin(true);
    ntsim_green_en_pin.writePin(false);
}

void tsim_set_green()
{
    ntsim_green_en_pin.writePin(true);
    tsim_red_en_pin.writePin(false);
}
} // namespace io
