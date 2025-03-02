#include "main.h"

#include "hw_gpios.h"
#include "hw_gpio.h"

const Gpio indicator_1 = { .port = INDICATOR1_GPIO_Port, .pin = INDICATOR1_Pin };
const Gpio indicator_2 = { .port = INDICATOR2_GPIO_Port, .pin = INDICATOR2_Pin };
const Gpio indicator_3 = { .port = INDICATOR3_GPIO_Port, .pin = INDICATOR3_Pin };
const Gpio dout_4      = { .port = DOUT4_GPIO_Port, .pin = DOUT4_Pin };