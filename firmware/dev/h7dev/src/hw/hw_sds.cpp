#include "hw_gpio.hpp"
#include "hw_sd.hpp"
#include "hw_gpio.hpp"
#include "main.h"

static const hw::Gpio sd_present{ GPIOA, GPIO_PIN_8 };
hw::SdCard            sd{ &hsd1, 0xFFFFFFFFU, sd_present };