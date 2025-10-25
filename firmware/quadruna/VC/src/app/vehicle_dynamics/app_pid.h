#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float Kb;
    float error;
    float filtered_derivative;
    float integral;
    float prev_input;
    float prev_output;
    float out_min;
    float out_max;
    bool back_calculation;
    uint32_t prev_time;
    uint32_t sample_time;
} PID;

typedef struct
{
    const float Kp;
    const float Ki;
    const float Kd;
    const float Kb;
    const float out_min;
    const float out_max;
    const bool back_calculation;
    const uint32_t sample_time;
} PID_Config;

void app_pid_init(PID *pid, const PID_Config *conf);

float app_pid_compute(PID *pid, float setpoint, float input);

void app_pid_requestReset(PID *pid);
