#pragma once
#include "app_pid.hpp"
#include "torque_allocator.hpp"

namespace app::tv::controllers::allocator
{
    const OptimizerWeights accel_ow  = 
    {
        .W_FX = std::sqrt(1.0f),
        .W_MZ = std::sqrt(0.5f),
        .W_R  = std::sqrt(10.0f)
    };

    const OptimizerWeights track_ow = 
    {
        .W_FX = std::sqrt(2.0f),
        .W_MZ = std::sqrt(15.0f),
        .W_R  = std::sqrt(5000.0f)
    };

    const OptimizerWeights vanilla_ow = 
    {
        .W_FX = std::sqrt(0.05f),
        .W_MZ = std::sqrt(0.05f),
        .W_R  = std::sqrt(5000.0f)
    };
}

namespace app::tv::controllers::dyrc
{
static constexpr app::PID::Config PID_DYRC_config{
    .Kp               = 400.0f,
    .Ki               = 0.0f,
    .Kd               = 0.0f,
    .Kb               = 0.0f,
    .Kff              = 0.0f,
    .smoothing_coeff  = 0.0f,
    .out_max          = 0.0f,
    .out_min          = 0.0f,
    .max_integral     = 0.0f,
    .min_integral     = 0.0f,
    .clamp_output     = false,
    .clamp_integral   = false,
    .back_calculation = false,
    .feed_forward     = false,
    .sample_time      = 0.01f // 10 ms sample time (100 Hz
};

static constexpr float DYRC_ku = 0.0f;
} // namespace app::tv::controllers::dyrc

namespace app::tv::controllers::speed_controller
{

static constexpr app::PID::Config PID_SPEED_config{
    .Kp               = 0.2f,
    .Ki               = 4.0f,
    .Kd               = 0.0f,
    .Kb               = 0.0f,
    .Kff              = 1.0f,
    .smoothing_coeff  = 1.0f,
    .out_max          = 21.0f,
    .out_min          = -15.0f,
    .max_integral     = 0.0f,
    .min_integral     = 0.0f,
    .clamp_output     = true,
    .clamp_integral   = false,
    .back_calculation = false,
    .feed_forward     = true,
    .sample_time      = 0.01f // 10 ms sample time (100 Hz)
};

} // namespace app::tv::controllers::speed_controller