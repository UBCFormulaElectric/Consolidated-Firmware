#pragma once

#include "app_brake.h"

#define BRAKE_PRESSED_PRESSURE_THRESHOLD_PSI (40.0f)
#define MIN_BRAKE_PRESSURE_PSI (0.0f)
#define MAX_BRAKE_PRESSURE_PSI (1000.0f)

/**
 * CAN BUS Broadcast Function
 */
void app_brake_broadcast(void);
