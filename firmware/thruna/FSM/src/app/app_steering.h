#pragma once

// The constants below were taken from the old FSM steering angle code
// and lower limits (#602)
// TODO: Calculate the new steering angle's voltage offset as well as the upper
#define MIN_STEERING_ANGLE_DEG (-130.0f)
#define MAX_STEERING_ANGLE_DEG (130.0f)

void app_steering_init(void);
void app_steering_destroy(void);
void app_steering_broadcast(void);