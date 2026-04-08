#pragma once

namespace app::tv::shared_datatypes::datatypes
{

template <typename T>
struct wheel_set
{
    T fl;
    T fr;
    T rl;
    T rr;
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

} // namespace app::tv::shared_datatypes::datatypes

namespace app::tv::datatypes
{
namespace datatypes = app::tv::shared_datatypes::datatypes;
}
