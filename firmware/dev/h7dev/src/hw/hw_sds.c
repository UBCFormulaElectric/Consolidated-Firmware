#include "hw_sds.h"
#include "main.h"

static const Gpio sd_present = { .pin = GPIO_PIN_8, .port = GPIOA };
SdCard            sd1        = { .hsd = &hsd1, .timeout = 0xFFFFFFFFU, .present_gpio = &sd_present };
