#include "io_switches.h"
#include "hw_gpios.h"

Switch push_drive_switch       = { .gpio = &push_drive_sig, .closed_state = true };
Switch regen_switch            = { .gpio = &regen_sig, .closed_state = true };
Switch torque_vectoring_switch = { .gpio = &torque_vectoring_sig, .closed_state = true };
Switch launch_control_switch   = { .gpio = &launch_control_sig, .closed_state = true };
Switch telem_switch            = { .gpio = &telem_sig, .closed_state = true };
