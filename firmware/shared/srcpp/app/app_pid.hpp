#pragma once
#include <cstdint>
#include <algorithm>

namespace app
{
class PID
{
  public:
    struct Config
    {
        float Kp;
        float Ki;
        float Kd;
        float Kb;

        float Kff;
        float smoothing_coeff;

        float out_max;
        float out_min;
        float max_integral;
        float min_integral;

        bool  clamp_output;
        bool  clamp_integral;
        bool  back_calculation;
        bool  feed_forward;
        float sample_time;
    };

    explicit PID(const Config &conf);

    float compute(float setpoint, float input, float disturbance);
    void  reset();
    float getIntegral();
    float getDerivative();

  private:
    // Gains and coefficients
    float Kp;
    float Ki;
    float Kd;
    float Kb;
    float Kff;             // often the inverse of Kp
    float smoothing_coeff; // set to 1 for no smoothing

    // Limits
    float out_max;
    float out_min;
    float max_integral;
    float min_integral;

    // Internal state
    float error            = 0.0f;
    float integral         = 0.0f;
    float derivative       = 0.0f;
    float prev_derivative  = 0.0f;
    float prev_disturbance = 0.0f;
    float prev_error       = 0.0f;

    bool  clamp_output;
    bool  clamp_integral;
    bool  back_calculation;
    bool  feed_forward;
    float sample_time;
};
} // namespace app
