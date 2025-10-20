#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
void io_fan_init(const Gpio *acc_fan_in, const Gpio *rad_fan_in);
#endif

void io_acc_fan_set(bool on);

void io_rad_fan_set(bool on);
