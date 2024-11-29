#include "app_pid.h"
#include "app_utils.h"

void app_pid_init(PID *pid, const PID_Config *conf)
{
    pid->Kp         = conf->Kp;
    pid->Ki         = conf->Ki;
    pid->Kd         = conf->Kd;
    pid->derivative_filtering_coeff = 0.0f; // will implement a filter later  
    pid->prev_input = 0.0f;
    pid->integral   = 0.0f;
    pid->integral_windup_min = NAN; 
    pid->integral_windup_max = NAN;
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

    if(app_guard_init(pid)){ // if initialized implement the gaurd
        if(pid->integral_windup_max > pid->integral && pid->integral > pid->integral_windup_min){
            pid->integral += pid->error;
        }
    } 
    else{
        pid->integral += pid->error;
    }
    
    // removing filtering for now 

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
    pid->error      = 0.0f; 
    pid->derivative = 0.0f;
}

bool app_guard_init(PID *pid){
    return !(isnanf(pid->integral_windup_max) || isnanf(pid->integral_windup_min)); 
}

