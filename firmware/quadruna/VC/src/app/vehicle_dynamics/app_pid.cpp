#include "app_pid.hpp"
#include "io_time.hpp" // for io_time_getCurrentMs()

namespace app
{
    PID::PID(const Config& conf) :
        Kp(conf.Kp), Ki(conf.Ki), Kd(conf.Kd), Kb(conf.Kb), 
        Kff(conf.Kff), smoothing_coeff(conf.smoothing_coeff),
        out_min(conf.out_min), out_max(conf.out_max), min_integral(conf.min_integral), max_integral(conf.max_integral),
        back_calculation(conf.back_calculation), feed_forward(conf.feed_forward), clamp_output(conf.clamp_output),
        sample_time(conf.sample_time)
    {
       
    }

    /**
     * PID controller effort implementation with derivative-on-error and discrete time handling. Includes:
     * - Integral Anti-wind up in the form of clamping and optional back calculation
     * - Derivative Filtering in the form of first order exponential smoothing
     * - Static Gain feed forward model
     * @param setpoint setpoint
     * @param input also "measurement"/"process variable"
     * @param disturbance feedforward/"disturbance" variable
     * @return controller output/"effort"
     */

    float PID::compute(float setpoint, float input, float disturbance)
    {

        error = setpoint - input;
        
        integral += error * sample_time;

        //Conditional Anti-Windup
        if (integral > max_integral){
            integral = std::clamp(integral, min_integral, max_integral);
        }
        
        // First order exponential smoothing (https://en.wikipedia.org/wiki/Exponential_smoothing)
        float raw_derivative = (error - prev_error) / sample_time;
        float filtered_derivative = smoothing_coeff * raw_derivative + (1 - smoothing_coeff) * prev_derivative; 

        //Feed Forward
        float u_ff = 0;
        if (feed_forward) {
            u_ff = Kff * disturbance;
        }
        
        float output = Kp * error + Ki * integral + Kd * filtered_derivative + u_ff;

        //Anti-Windup with back calculation (https://www.cds.caltech.edu/~murray/books/AM08/pdf/am08-pid_04Mar10.pdf) equation 10.16
        if (back_calculation) {
            if(clamp_output){}
            float out_clamp = std::clamp(output, out_min, out_max);
            integral += Kb * (out_clamp - output) * sample_time;

        }   
        
        if(clamp_output) {
            output = std::clamp(output, out_min, out_max);
        }
        
        prev_error = error;
        prev_disturbance =  disturbance;
        prev_derivative = filtered_derivative;

        return output;
        
    }

    void PID::reset()
    {
        integral = 0.0f;
        prev_derivative = 0.0f;
        prev_disturbance = 0.0f;
        prev_error = 0.0f;
    }
}