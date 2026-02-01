#include "io_tsim.hpp"
#include "hw_gpios.hpp"

namespace io
{
void tsim_set_off()
{
    hw::tsim_red_en_pin.writePin(false);
    hw::ntsim_green_en_pin.writePin(false);
}

void tsim_set_red()
{
    hw::tsim_red_en_pin.writePin(true);
    hw::ntsim_green_en_pin.writePin(false);
}

void tsim_set_green()
{
    hw::ntsim_green_en_pin.writePin(true);
    hw::tsim_red_en_pin.writePin(false);
}
} // namespace io
