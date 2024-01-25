#pragma once

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float error;
    float derivative;
    float integral;
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

void App_PID_Init(PID* pid, const PID_Config* conf);

float App_PID_Compute(PID* pid, float setpoint, float input);

void App_PID_RequestReset(PID* pid);
