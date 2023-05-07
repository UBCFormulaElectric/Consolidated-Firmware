#pragma once

#define MOTOR_EFFICIENCY_LUT_INTERVAL (500U)
#define MOTOR_EFFICIENCY_LUT_SIZE (14U)

// Motor efficiency LUT with interval of 500rpm
static const float motor_efficiency_lut[MOTOR_EFFICIENCY_LUT_SIZE] = {
    0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
