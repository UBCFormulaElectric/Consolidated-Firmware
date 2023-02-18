#pragma once


typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float prev_error;
    float integral;
    float out_min;
    float out_max;
} PID;

float APP_PIDInit(PID *pid, float Kp, float Ki, float Kd, float out_min, float out_max);

void App_PIDCompute(PID *pid, float setpoint, float input, float dt);

void APP_PIDRequestReset(PID *pid);

