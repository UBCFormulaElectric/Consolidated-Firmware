#pragma once

#include "io_switch.hpp"
#include "hw_gpios.hpp"
namespace io::switches
{
extern Switch torque_vectoring_sw;
extern Switch launch_control_sw;
extern Switch regen_sw;
extern Switch start_sw;
extern Switch telem_sw;
} // namespace io::switches