#include "app_pid.h"
#include "app_utils.h"

void app_pid_init(PID *pid, const PID_Config *conf)
{
    pid->Kp         = conf->Kp;
    pid->Ki         = conf->Ki;
    pid->Kd         = conf->Kd;
    pid->derivative_filtering_coeff = NAN; 
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

    if(app_gaurd_init(pid)){ // if initialized implement the gaurd
        if(pid->integral_windup_max > pid->integral && pid->integral > pid->integral_windup_min){
            pid->integral += pid->error;
        }
    } 
    else{
        pid->integral += pid->error;
    }

    if(app_filter_init(pid)){
        pid->derivative = pid->derivative_filtering_coeff * (pid->derivative / (1 + pid->derivative_filtering_coeff));
    }
    else{
        pid->derivative = (input - pid->prev_input);
    }

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

bool app_gaurd_init(PID *pid){
    return (pid->integral_windup_max != NAN && pid->integral_windup_min != NAN); 
}

bool app_filter_init(PID *pid){
    return (pid->derivative_filtering_coeff != NAN); 
}
