#pragma once

#include <stdint.h>

extern float vehicle_velocity;

/*
 * Broadcast sensor outputs over CAN.
 */
void app_sbgEllipse_broadcast(void);
