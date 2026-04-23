#include "io_switches.hpp"
#include "hw_gpios.hpp"

namespace io::switches
{
static constexpr uint8_t SW_DEBOUNCE_TICKS = 5;

Switch torque_vectoring_sw(SW_DEBOUNCE_TICKS, torque_vectoring_sig);
Switch launch_control_sw(SW_DEBOUNCE_TICKS, launch_control_sig);
Switch regen_sw(SW_DEBOUNCE_TICKS, regen_sig);
Switch start_sw(SW_DEBOUNCE_TICKS, push_drive_sig);
Switch telem_sw(SW_DEBOUNCE_TICKS, telem_sig);
} // namespace io::switches