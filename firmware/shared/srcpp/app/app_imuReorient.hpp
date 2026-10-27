#pragma once
#include <cmath>
#include "util_errorCodes.hpp"
#include <array>

namespace app::reorient
{
using namespace std;
using Rotation = array<array<float, 3>, 3>;

struct imu_reorient
{
    // IMU manufacturer biases
    const float x_bias;
    const float y_bias;
    const float z_bias;
    const float roll_bias;
    const float pitch_bias;
    const float yaw_bias;

    Rotation R;

    imu_reorient(
        const float x_bias_in,
        const float y_bias_in,
        const float z_bias_in,
        const float roll_bias_in,
        const float pitch_bias_in,
        const float yaw_bias_in,
        const float x_mean,
        const float y_mean,
        const float z_mean,
        const float xy_ratio)
      : x_bias(x_bias_in),
        y_bias(y_bias_in),
        z_bias(z_bias_in),
        roll_bias(roll_bias_in),
        pitch_bias(pitch_bias_in),
        yaw_bias(yaw_bias_in)
    {
        const float corrected_x = x_mean - x_bias;
        const float corrected_y = y_mean - y_bias;
        const float corrected_z = z_mean - z_bias;

        const float alpha = atan2(corrected_y, corrected_z);                      // angle to rotate by about x axis
        const float beta  = -atan2(corrected_x, hypot(corrected_y, corrected_z)); // angle to rotate by about y axis
        const float gamma = -atan(xy_ratio);

        const float cos_alpha = cos(alpha);
        const float cos_beta  = cos(beta);
        const float cos_gamma = cos(gamma);
        const float sin_alpha = sin(alpha);
        const float sin_beta  = sin(beta);
        const float sin_gamma = sin(gamma);

        R = {
            { { { cos_beta * cos_gamma, sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma,
                  cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma } },
              { { cos_beta * sin_gamma, sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,
                  cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma } },
              { { -sin_beta, sin_alpha * cos_beta, cos_alpha * cos_beta } } }
        }; // rotation matrix R = Rz(gamma) * Ry(beta) * Rx(alpha)
    }
};

struct imu_readings
{
    result<float> accelX;
    result<float> accelY;
    result<float> accelZ;
    result<float> gyroX;
    result<float> gyroY;
    result<float> gyroZ;

    inline constexpr bool hasFault()
    {
        return not(
            accelX.has_value() && accelY.has_value() && accelZ.has_value() && gyroX.has_value() && gyroY.has_value() &&
            gyroZ.has_value());
    }
};

imu_readings apply(imu_reorient &reorient, imu_readings &imu);
} // namespace app::reorient