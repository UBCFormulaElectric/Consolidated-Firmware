#pragma once

namespace app::tv::datatypes::datatypes
{
struct wheel_set
{
    float fl = 0.0f;
    float fr = 0.0f;
    float rl = 0.0f;
    float rr = 0.0f;
};

struct VehicleState
{
    float v_x_mps        = 0.0f;
    float v_y_mps        = 0.0f;
    float yaw_rate_radps = 0.0f;
    float steer_ang_rad  = 0.0f;
    float a_x_mps2       = 0.0f;
    float a_y_mps2       = 0.0f;
};
} // namespace app::tv::datatypes::datatypes
