#include "app_pid.hpp"
#include "io_time.hpp" // for io_time_getCurrentMs()


namespace app
{
    PID::PID(const Config& conf) : Kp(conf.Kp), Ki(conf.Ki), Kd(conf.Kd), Kb(conf.Kb),
        out_min(conf.out_min), out_max(conf.out_max),
        back_calculation(conf.back_calculation),
        sample_time(conf.sample_time)
    {
        prev_time = io::time::getCurrentMs() - sample_time;
        prev_derivative = 0;
    }

    /**
     * PID controller effort implementation with derivative-on-measurement and discrete time handling. Includes:
     * - Integral Anti-wind up in the form of clamping and optional back calculation
     * - Derivative Filtering in the form of
     * @param setpoint setpoint
     * @param input also "measurement"/"process variable"
     * @return controller output/"effort"
     */

    float PID::compute(float setpoint, float input)
    {
        uint32_t current_time = io::time::getCurrentMs();
        float dt = static_cast<float>(current_time - prev_time);
        prev_time = current_time;

        bool integrate = true; //Assume we want to integrate

        if (dt >= sample_time || prev_output == 0.0f)
        {
            error = setpoint - input;

            // For clamping (default anti-windup)
            if (integrate)
                integral += error * dt;
            else
                integral = 0;

            float raw_derivative = (input - prev_input) / dt;
            
            // Derivative Filtering
            float alpha = N/ (N + 1/sample_time);
            float beta = Kd*N/(N + 1/sample_time);
            filtered_derivative = alpha * prev_derivative - beta * (input - prev_input);

            float u_calc = Kp * error + Ki * integral - Kd * filtered_derivative;
            float u_actual = std::clamp(u_calc, out_min, out_max);

            integrate = (u_calc == u_actual);

            //for optional back calculation
            if (back_calculation) {
                integral += Kb * (u_actual - u_calc);
            }   

            prev_input = input;
            prev_output = u_actual;
            return u_actual;
        }
        else
        {
            return prev_output;
        }
    }

    void PID::reset()
    {
        prev_input = 0.0f;
        integral = 0.0f;
        prev_output = 0.0f;
    }
}