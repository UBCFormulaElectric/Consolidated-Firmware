#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    // Gains and Coefficients
    float Kp;
    float Ki;
    float Kd;
    float Kb;
    float Kff; // often the inverse of Kp
    float smoothing_coeff;

    // Limits
    float out_max;
    float out_min;
    float max_integral;
    float min_integral;

    // Internal State
    float error;
    float integral;
    float derivative;
    float prev_derivative;
    float prev_disturbance;
    float prev_error;

    bool  clamp_output;
    bool  clamp_integral;
    bool  back_calculation;
    bool  feed_forward;
    bool  clamp_output;
    bool  clamp_integral;
    bool  back_calculation;
    bool  feed_forward;
    float sample_time;
} PID;

typedef struct
{
    const float Kp;
    const float Ki;
    const float Kd;
    const float Kb;

    const float Kff;
    const float smoothing_coeff;

    const float out_max;
    const float out_min;
    const float max_integral;
    const float min_integral;

    const bool  clamp_output;
    const bool  clamp_integral;
    const bool  back_calculation;
    const bool  feed_forward;
    const bool  clamp_output;
    const bool  clamp_integral;
    const bool  back_calculation;
    const bool  feed_forward;
    const float sample_time;
} PID_Config;

void app_pid_init(PID *pid, const PID_Config *conf);

float app_pid_compute(PID *pid, float setpoint, float input, float disturbance);
void  app_pid_requestReset(PID *pid);
