#include "hw_gpios.h"
#include "main.h"
#include "app_utils.h"

namespace hw::gpio

{
const Gpio brake_ocsc_ok_3v3{ BRAKE_OCSC_OK_3V3_GPIO_Port, BRAKE_OCSC_OK_3V3_Pin};
const Gpio n_chimera_pin { NCHIMERA_GPIO_Port,NCHIMERA_Pin };
const Gpio led { LED_GPIO_Port, LED_Pin };
const Gpio nbspsd_brake_pressed_3v3{ NBSPD_BRAKE_PRESSED_3V3_GPIO_Port, NBSPD_BRAKE_PRESSED_3V3_Pin };
const Gpio nprogram_3v3 { NPROGRAM_3V3_GPIO_Port, NPROGRAM_3V3_Pin };
const Gpio fsm_shdn { FSM_SHDN_GPIO_Port, FSM_SHDN_Pin };
} // namespace hw::gpio
