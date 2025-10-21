#pragma once

// TODO: angles need to be validated
#define MIN_STEERING_ANGLE_DEG (-95.0f)
#define MAX_STEERING_ANGLE_DEG (95.0f)

/**
 * CAN BUS Broadcast Function
 */
void app_steering_broadcast(void);
