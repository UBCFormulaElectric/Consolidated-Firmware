#include "io_switches.h"
#include "main.h"

Switch push_drive_switch = { .gpio         = { .port = PUSH_DRIVE_SIG_GPIO_Port, .pin = PUSH_DRIVE_SIG_Pin },
                             .closed_state = true };
Switch regen_switch      = { .gpio = { .port = REGEN_SIG_GPIO_Port, .pin = REGEN_SIG_Pin }, .closed_state = true };
Switch torque_vectoring_switch = { .gpio = { .port = TORQUE_VECTORING_SIG_GPIO_Port, .pin = TORQUE_VECTORING_SIG_Pin },
                                   .closed_state = true };
Switch launch_control_switch   = { .gpio = { .port = LAUNCH_CONTROL_SIG_GPIO_Port, .pin = LAUNCH_CONTROL_SIG_Pin },
                                   .closed_state = true };
Switch telem_switch            = { .gpio = { .port = TELEM_SIG_GPIO_Port, .pin = LED_Pin }, .closed_state = true };
