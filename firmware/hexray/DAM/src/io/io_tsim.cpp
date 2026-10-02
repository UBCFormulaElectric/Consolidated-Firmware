#include "io_tsim.hpp"
#include "hw_gpios.hpp"

namespace io::tsim
{
// Note: ntsim_green_en_pin is active-low (nTSIM_GRN_EN), so writePin(true) disables green.
void set_off()
{
    tsim_red_en_pin.writePin(false);
    ntsim_green_en_pin.writePin(true);
}

void set_red()
{
    ntsim_green_en_pin.writePin(true);
    tsim_red_en_pin.writePin(true);
}

void set_green()
{
    tsim_red_en_pin.writePin(false);
    ntsim_green_en_pin.writePin(false);
}
} // namespace io::tsim
