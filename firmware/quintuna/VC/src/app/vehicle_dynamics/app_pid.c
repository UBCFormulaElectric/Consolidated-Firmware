#include "app_pid.h"
#include "app_utils.h"
#include <assert.h>

void app_pid_init(PID *pid, const PID_Config *conf)
{
    pid->Kp                 = conf->Kp;
    pid->Ki                 = conf->Ki;
    pid->Kd                 = conf->Kd;
    pid->Kb                 = conf->Kb;
    pid->Kff                = conf->Kff;
    pid->out_min            = conf->out_min;
    pid->out_max            = conf->out_max;
    pid->smoothing_coeff    = conf->smoothing_coeff;
    pid->max_integral       = conf->max_integral;
    pid->min_integral       = conf->min_integral;
    pid->clamp_integral     = conf->clamp_integral;
    pid->clamp_output       = conf->clamp_output;
    pid->back_calculation   = conf->back_calculation;
    pid->feed_forward       = conf->feed_forward;
    pid->sample_time        = conf->sample_time;
    
    pid->prev_error         = 0.0f;
    pid->prev_derivative    = 0.0f;
    pid->prev_disturbance   = 0.0f;
    pid->derivative         = 0.0f;
    pid->integral           = 0.0f;
    pid->error              = 0.0f;

    assert(pid->out_min > pid-> out_min);
    assert(pid->max_integral > pid->min_integral);
    assert(pid->sample_time > 0);
}

/**
 * PID controller effort implementation with derivative-on-error and discrete time handling. Make sure to Scale 
 * your inputs accordingly.Includes:
 * - Integral Anti-wind up in the form of clamping and optional back calculation
 * - Derivative Filtering in the form of first order exponential smoothing
 * - Static Gain feed forward model
 * @param pid data store
 * @param setpoint setpoint
 * @param input also "measurement"/"process variable"
 * @param disturbance feedforward/"disturbance" variable
 * @return controller output/"effort"
 */
float app_pid_compute(PID *pid, const float setpoint, const float input, float disturbance)
{

    pid->error = setpoint - input;

    pid->integral += pid->error * pid->sample_time;
    
    //Conditional Anti-Windup 
    if (pid->clamp_integral) pid->integral = CLAMP(pid->integral, pid->min_integral, pid->max_integral);
    
    // First order exponential smoothing on derivative (https://en.wikipedia.org/wiki/Exponential_smoothing)
    float raw_derivative = (pid->error - pid->prev_error) / pid->sample_time;
    float derivative = pid->smoothing_coeff * raw_derivative + (1-pid->smoothing_coeff) * pid->prev_derivative;    
    
    //Feed Forward
    float u_ff = 0;
    if (pid->feed_forward) { 
        u_ff = pid->Kff * disturbance;
    }

    float output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * derivative + u_ff;

    //Anti-Windup with back calculation (https://www.cds.caltech.edu/~murray/books/AM08/pdf/am08-pid_04Mar10.pdf) equation 10.16
    if (pid->back_calculation ){
        float out_clamp = CLAMP(output, pid->out_min, pid->out_max);
        pid->integral +=  pid->sample_time*pid->Ki*pid->error + pid->Kb * (out_clamp - output);
    }

    if (pid->clamp_output){
        output = CLAMP(output, pid->out_min, pid->out_max);
    }

    pid->prev_error = pid->error;
    pid->prev_disturbance =  disturbance;
    pid->prev_derivative = derivative;

    return output;

}

void app_pid_requestReset(PID *pid)
{
    pid->integral   = 0.0f;
    pid->prev_derivative = 0.0f;
    pid->prev_disturbance = 0.0f;
    pid->prev_error = 0.0f;
}