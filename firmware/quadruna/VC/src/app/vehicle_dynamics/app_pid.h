#pragma once
#include "stdbool.h"

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float error;
    float derivative;
    float derivative_filtering_coeff; 
    float integral;
    float integral_windup_min;
    float integral_windup_max;
    float prev_input;
    float out_min;
    float out_max;
} PID;

typedef struct
{
    const float Kp;
    const float Ki;
    const float Kd;
    const float out_min;
    const float out_max;
} PID_Config;

void app_pid_init(PID *pid, const PID_Config *conf);

float app_pid_compute(PID *pid, float setpoint, float input);

void app_pid_requestReset(PID *pid);

bool app_gaurd_init(PID *pid); 

bool app_filter_init(PID *pid);