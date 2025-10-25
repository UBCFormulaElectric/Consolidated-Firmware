#include "app_pid.h"
#include "app_utils.h"

void app_pid_init(PID *pid, const PID_Config *conf)
{
    pid->Kp          = conf->Kp;
    pid->Ki          = conf->Ki;
    pid->Kd          = conf->Kd;
    pid->Kb          = conf->Kb;
    pid->prev_input  = 0.0f;
    pid->prev_output = 0.0f;
    pid->integral    = 0.0f;
    pid->out_min     = conf->out_min;
    pid->out_max     = conf->out_max;
    pid->back_calculation = conf->back_calculation;
    pid->prev_time   = io_time_getCurrentMs() - conf->sample_time;
}

/**
 * PID controller effort implementation with derivative-on-measurement and discrete time handling
 * @param pid data store
 * @param setpoint setpoint
 * @param input also "measurement"/"process variable"
 * @return controller output/"effort"
 */
float app_pid_compute(PID *pid, const float setpoint, const float input)
{
    uint32_t current_time = io_time_getCurrentMs();
    float dt = current_time - pid->prev_time;
    pid->prev_time = current_time;
    bool integrate =  true;

    if (dt >= pid->sample_time | pid->prev_output == 0)
    {       
        pid->error = setpoint - input;
        
        if(integrate){
            pid->integral += pid->error * dt;
        }else{
            pid->integral = 0;
        }

        float raw_derivative = (input - pid->prev_input) / dt;
        pid->prev_input = input;        

        float u_calc = pid->Kp * pid->error + pid->Ki * pid->integral - pid->Kd * pid->filtered_derivative;

        float u_actual  = CLAMP(u_calc, pid->out_min, pid->out_max);

        //Saturation check (is ucalc - u_actual = 0?)
        if(u_calc != u_actual){
            integrate = false;  
        }else{
            integrate = true;
        }

        // Update integral with back calculation
        if (pid->back_calculation){
            pid-> integral = pid->integral + pid->Kb*(u_actual - u_calc);
        }

        return u_actual;
    } else {
        return pid->prev_output;
    }
}

void app_pid_requestReset(PID *pid)
{
    pid->prev_input = 0.0f;
    pid->integral   = 0.0f;
}