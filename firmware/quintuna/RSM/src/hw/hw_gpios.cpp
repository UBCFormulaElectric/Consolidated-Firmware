#include "hw_Gpios.h"
#include "main.h"
#include "app.utils.h"

namespace hw::gpio
{
const Gpio n_chimera_pin{ NCHIMERA_GPIO_Port, NCHIMERA_Pin };
const Gpio led_pin{ LED_GPIO_Port, LED_Pin };
const Gpio rad_fan_en_pin{ RAD_FAN_EN_GPIO_Port, RAD_FAN_EN_Pin };
const Gpio fr_stby_pin{ FR_STBY_GPIO_Port, FR_STBY_Pin };
const Gpio brake_light_en_pin{ BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin };
const Gpio acc_fan_en_pin{ ACC_FAN_EN_GPIO_Port, ACC_FAN_EN_Pin };
const Gpio n_program_pin{ NProgram_3V3_GPIO_Port, NProgram_3V3_Pin };
const Gpio acc_fan_pin{ ACC_FAN_EN_GPIO_Port, RAD_FAN_EN_Pin };
const Gpio rad_fan_pin{ RAD_FAN_EN_GPIO_Port, ACC_FAN_EN_Pin };
} // namespace hw::gpio
