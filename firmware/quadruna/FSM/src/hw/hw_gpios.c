#include "hw_gpios.h"
#include "main.h"

const Gpio brake_ocsc_ok_3v3       = { .port = BRAKE_OCSC_OK_3V3_GPIO_Port, .pin = BRAKE_OCSC_OK_3V3_Pin };
const Gpio n_chimera_pin           = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
const Gpio led_gpio_pin            = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio nbspd_brake_pressed_3v3 = { .port = NBSPD_BRAKE_PRESSED_3V3_GPIO_Port, .pin = NBSPD_BRAKE_PRESSED_3V3_Pin };
const Gpio nprogram_3v3            = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
const Gpio fsm_shdn                = { .port = FSM_SHDN_GPIO_Port, .pin = FSM_SHDN_Pin };
