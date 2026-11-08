#pragma once
#include "app_pid.hpp"

namespace app::tv::controllers::dyrc
{
static constexpr app::PID::Config PID_DYRC_config{
    .Kp               = 9000.0f, // tuned on the driver_sim autocross lap (with W_R = 20 in the allocator)
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
    .sample_time      = 0.05f // 50 ms sample time (20 Hz)
};

static constexpr float DYRC_ku = -0.0e-3f;
// The reference yaw rate is limited to what the tires can hold in a steady turn: |r_ref| <= mu * g / v. Without it,
// large steer at speed asks for a yaw moment the tires cannot give. About the tire model's steady limit (1.6-1.7 g).
static constexpr float DYRC_MU_LIMIT = 1.6f;
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
    .max_integral     = 21.0f / 4.0f,
    .min_integral     = -15.0f / 4.0f,
    .clamp_output     = true,
    .clamp_integral   = true,
    .back_calculation = false,
    .feed_forward     = true,
    .sample_time      = 0.01f // 10 ms sample time (100 Hz)
};

// The car counts as stopped (speed controllers reset) while the estimated |v_x| and |v_y| are both below this. The
// estimate is never exactly 0; at standstill the zero-speed update holds it within a few mm/s.
static constexpr float STOPPED_SPEED_MPS = 0.05f;
} // namespace app::tv::controllers::speed_controller

// downforce * mu / (mass * vx)
// ay = vx * r
// r = ay / vx
// r = (downforce * mu / (mass)) / vx