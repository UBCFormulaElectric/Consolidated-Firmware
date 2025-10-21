#include "io/switches.h"
#include "hw/gpios.h"

Switch push_drive_switch       = { .gpio = &push_drive_sig, .closed_state = true };
Switch regen_switch            = { .gpio = &regen_sig, .closed_state = true };
Switch vanilla_override_switch = { .gpio = &vanilla_sig, .closed_state = true };
Switch launch_control_switch   = { .gpio = &launch_control_sig, .closed_state = true };
Switch telem_switch            = { .gpio = &telem_sig, .closed_state = true };
