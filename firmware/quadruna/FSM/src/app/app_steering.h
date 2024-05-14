#pragma once

// The constants below were taken from the old FSM steering angle code
// and lower limits (#602)
#define MIN_STEERING_ANGLE_DEG (-95.0f)
#define MAX_STEERING_ANGLE_DEG (95.0f)

/**
 * CAN BUS Broadcast Function
 */
void app_steering_broadcast(void);