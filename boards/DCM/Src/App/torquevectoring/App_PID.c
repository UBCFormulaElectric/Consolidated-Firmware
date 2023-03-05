#include "torquevectoring/App_PID.h"

void App_PID_Init(PID *pid, const PID_Config *conf)
{
    pid->Kp         = conf->Kp;
    pid->Ki         = conf->Ki;
    pid->Kd         = conf->Kd;
    pid->prev_error = 0.0f;
    pid->integral   = 0.0f;
    pid->out_min    = conf->out_min;
    pid->out_max    = conf->out_max;
}

float App_PID_Compute(PID *pid, float setpoint, float input)
{
    float error = setpoint - input;
    pid->integral += error;
    float derivative = (error - pid->prev_error);
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

void App_PID_RequestReset(PID *pid)
{
    pid->prev_error = 0;
    pid->integral   = 0;
}
