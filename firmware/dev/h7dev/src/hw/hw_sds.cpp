#include "hw_gpio.hpp"
#include "hw_sd.hpp"
#include "main.h"
#include <cassert>

static const hw::Gpio sd_present{ GPIOA, GPIO_PIN_8 };
hw::SdCard            sd{ &hsd1, 0xFFFFFFFFU, sd_present };

const hw::SdCard &hw::getSdFromHandle(SD_HandleTypeDef *hsd)
{
    assert(hsd == sd.getHsd());
    return sd;
}