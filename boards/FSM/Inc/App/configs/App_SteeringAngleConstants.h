#pragma once

// The constants below were taken from the old FSM steering angle code
// TODO: Calculate the new steering angle's voltage offset as well as the upper
// and lower limits (#602)

#define STEERING_ANGLE_VOLTAGE_OFFSET 1.9f
#define MIN_STEERING_ANGLE -110.0f
#define MAX_STEERING_ANGLE 110.0f
