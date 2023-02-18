#include "torquevectoring/App_PID.h"

void APP_PID_Init(PID *pid, float Kp, float Ki, float Kd, float out_min, float out_max)
{
    pid->Kp         = Kp;
    pid->Ki         = Ki;
    pid->Kd         = Kd;
    pid->prev_error = 0;
    pid->integral   = 0;
    pid->out_min    = out_min;
    pid->out_max    = out_max;
}

float APP_PID_Compute(PID *pid, float setpoint, float input, float dt)
{
    float error = setpoint - input;
    pid->integral += error * dt;
    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error  = error;
    float output     = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    if (output < pid->out_min)
    {
        output = pid->out_min;
    }
    else if (output > pid->out_max)
    {
        output = pid->out_max;
    }

    return output;
}

void APP_PID_RequestReset(PID *pid)
{
    pid->prev_error = 0;
    pid->integral   = 0;
}
