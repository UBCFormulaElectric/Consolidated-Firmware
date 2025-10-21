#pragma once

#define MIN_BRAKE_PRESSURE_PSI (0.0f)
#define MAX_BRAKE_PRESSURE_PSI (1000.0f)

/*
 * Brodcast brake info on CAN
 */
void app_brake_broadcast(void);