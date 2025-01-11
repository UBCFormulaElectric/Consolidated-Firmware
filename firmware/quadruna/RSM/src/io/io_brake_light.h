#pragma once

#ifdef TARGET_EMBEDDED
#include "io_led.h"
void io_brake_light_init(const BinaryLed *brake_light);
#endif

void io_brake_light_set(bool val);
