#include "app_pid.hpp"

static constexpr app::PID::Config PID_DYRC_CONFIG
{
    .Kp = 0.25f,
    .Ki = 0.0f,
    .Kd = 0.0f,
    .Kb = 0.0f,
    .Kff = 0.0f,
    .smoothing_coeff = 0.0f,
    .out_max = 0.0f,
    .out_min = 0.0f,
    .max_integral = 0.0f,
    .min_integral = 0.0f,
    .clamp_output = false,
    .clamp_integral = false,
    .back_calculation = false,
    .feed_forward = false,
    .sample_time = 0.01f // 10 ms sample time (100 Hz
};