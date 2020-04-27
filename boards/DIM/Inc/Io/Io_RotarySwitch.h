#pragma once

#include <stdint.h>
#include "App_RotarySwitch.h"

void Io_RotarySwitch_Init(void);
void Io_RotarySwitch_ExtiCallback(uint16_t GPIO_Pin);
enum DriveMode Io_RotarySwitch_GetDriveMode(void);
