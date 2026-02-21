#pragma once
#include <cstdint>
#include <algorithm>
#include <cassert>

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

    explicit constexpr PID(const Config &conf)
  : Kp(conf.Kp),
    Ki(conf.Ki),
    Kd(conf.Kd),
    Kb(conf.Kb),
    Kff(conf.Kff),
    smoothing_coeff(conf.smoothing_coeff),
    out_max(conf.out_max),
    out_min(conf.out_min),
    max_integral(conf.max_integral),
    min_integral(conf.min_integral),
    clamp_output(conf.clamp_output),
    clamp_integral(conf.clamp_integral),
    back_calculation(conf.back_calculation),
    feed_forward(conf.feed_forward),
    sample_time(conf.sample_time)
{
    assert(out_max >= out_min);
    assert(max_integral >= min_integral);
    assert(sample_time > 0.0f);
}

    [[nodiscard]] float compute(const float setpoint, const float input, const float disturbance = 0.0f);
    void  reset();
    [[nodiscard]] float getIntegral();
    [[nodiscard]] float getDerivative();

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
