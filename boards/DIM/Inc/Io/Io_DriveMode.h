#pragma once

#include <stdint.h>
#include "App_DriveMode.h"

void Io_RotarySwitch_ExtiCallback(uint16_t GPIO_Pin);
enum DriveMode Io_RotarySwitch_GetDriveMode(void);
