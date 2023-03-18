#include "torquevectoring/App_PID.h"
#include "torquevectoring/App_TorqueVectoringUtils.h"

void App_PID_Init(PID *pid, const PID_Config *conf)
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
float App_PID_Compute(PID *pid, float setpoint, float input)
{
    float error = setpoint - input;
    pid->integral += error;
    float derivative = (input - pid->prev_input);
    pid->prev_input  = input;
    float output     = pid->Kp * error + pid->Ki * pid->integral - pid->Kd * derivative;

    output = CLAMP(output, pid->out_min, pid->out_max);
    return output;
}

void App_PID_RequestReset(PID *pid)
{
    pid->prev_input = 0.0f;
    pid->integral   = 0.0f;
}
