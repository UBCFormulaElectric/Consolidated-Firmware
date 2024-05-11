#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
void io_fans_init(const Gpio *acc_fan_in, const Gpio *rad_fan_in);
#endif

void io_fans_accfan_set(bool val);
void io_fans_radfan_set(bool val);
