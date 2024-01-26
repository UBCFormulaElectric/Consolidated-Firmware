#include "app_pid.h"
#include "App_SharedMacros.h"

void app_pid_init(PID *pid, const PID_Config *conf)
{
    pid->Kp         = conf->Kp;
    pid->Ki         = conf->Ki;
    pid->Kd         = conf->Kd;
    pid->prev_input = 0.0f;
    pid->integral   = 0.0f;
    pid->out_min    = conf->out_min;
    pid->out_max    = conf->out_max;
}

/**
 * PID controller effort implementation with derivative-on-measurement
 * @param pid data store
 * @param setpoint setpoint
 * @param input also "measurement"/"process variable"
 * @return controller output/"effort"
 */
float app_pid_compute(PID *pid, float setpoint, float input)
{
    pid->error = setpoint - input;
    pid->integral += pid->error;
    pid->derivative = (input - pid->prev_input);
    pid->prev_input = input;
    float output    = pid->Kp * pid->error + pid->Ki * pid->integral - pid->Kd * pid->derivative;

    output = CLAMP(output, pid->out_min, pid->out_max);
    return output;
}

void app_pid_requestReset(PID *pid)
{
    pid->prev_input = 0.0f;
    pid->integral   = 0.0f;
}
