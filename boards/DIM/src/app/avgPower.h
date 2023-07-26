#pragma once

#include <stdbool.h>
#include <stdint.h>

void avgPower_init(void);
float avgPower_update(float instant_power);
void avgPower_reset(void);
