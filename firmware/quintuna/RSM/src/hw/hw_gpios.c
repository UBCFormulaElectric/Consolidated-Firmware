#include "hw_gpios.h"
#include "main.h"

// const Gpio n_chimera_pin      = { NCHIMERA_GPIO_Port, NCHIMERA_Pin };
const Gpio led_pin            = { LED_GPIO_Port, LED_Pin };
const Gpio brake_light_en_pin = { BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin };
const Gpio boot_pin           = { BOOT_GPIO_Port, BOOT_Pin };
// const Gpio acc_fan_en_pin     = { ACC_FAN_EN_GPIO_Port, ACC_FAN_EN_Pin };
// const Gpio n_program_pin      = { NProgram_3V3_GPIO_Port, NProgram_3V3_Pin };
// const Gpio acc_fan_pin        = { ACC_FAN_EN_GPIO_Port, RAD_FAN_EN_Pin };
// const Gpio rad_fan_pin        = { RAD_FAN_EN_GPIO_Port, ACC_FAN_EN_Pin };

// review with hardware