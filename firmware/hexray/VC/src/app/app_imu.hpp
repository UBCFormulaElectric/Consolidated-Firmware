#pragma once

// refactor needed
#include <expected>
#include "io_imus.hpp"
#include "io_imu.hpp"
#ifdef TARGET_EMBEDDED
#include "io_efuse.hpp"
#endif

namespace app::imus
{
struct VCImuResults
{
    result<float> accel_x_res;
    result<float> accel_y_res;
    result<float> accel_z_res;
    result<float> gyro_x_res;
    result<float> gyro_y_res;
    result<float> gyro_z_res;

    inline constexpr bool hasFault()
    {
        return not(
            accel_x_res.has_value() && accel_y_res.has_value() && accel_z_res.has_value() && gyro_x_res.has_value() &&
            gyro_y_res.has_value() && gyro_z_res.has_value());
    }
};

void          init();
void          broadcast();
result<float> getAccelX();
result<float> getAccelY();
result<float> getAccelZ();
result<float> getGyroX();
result<float> getGyroY();
result<float> getGyroZ();
bool          initSuccess();
// TODO: transformation matrix to reorient IMU to vehicle body frame (do for FSM and RSM as well)
} // namespace app::imus
