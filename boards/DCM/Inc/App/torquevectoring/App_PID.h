#pragma once

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float prev_error;
    float integral;
    float out_min;
    float out_max;
} PID;

void APP_PID_Init(PID *pid, float Kp, float Ki, float Kd, float out_min, float out_max);

void App_PID_Compute(PID *pid, float setpoint, float input, float dt);

void APP_PID_RequestReset(PID *pid);
