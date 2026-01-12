#include "hw_gpios.h"
#include "main.h"

const Gpio led_pin  = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio boot_pin = {.port = BOOT_GPIO_Port, .pin = BOOT_Pin};