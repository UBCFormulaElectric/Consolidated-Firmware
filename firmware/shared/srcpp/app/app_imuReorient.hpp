#pragma once
#include <cmath>
#include "util_errorCodes.hpp"
#include <array>

namespace app::reorient
{
using namespace std;
using Rotation = array<array<float, 3>, 3>;

struct ImuReorient
{
    // IMU manufacturer biases
    const float x_bias;
    const float y_bias;
    const float z_bias;
    const float roll_bias;
    const float pitch_bias;
    const float yaw_bias;

    Rotation R;

    ImuReorient(
        const float x_bias_in     = 0.0f,
        const float y_bias_in     = 0.0f,
        const float z_bias_in     = 0.0f,
        const float roll_bias_in  = 0.0f,
        const float pitch_bias_in = 0.0f,
        const float yaw_bias_in   = 0.0f,
        const float x_mean        = 0.0f,
        const float y_mean        = 0.0f,
        const float z_mean        = 1.0f,
        const float xy_ratio      = 0.0f)
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

        R = { { { { cos_beta * cos_gamma, sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma,
                    cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma } },
                { { cos_beta * sin_gamma, sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,
                    cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma } },
                { { -sin_beta, sin_alpha * cos_beta, cos_alpha * cos_beta } } } };
        /*
         * Rotation matrix R: maps IMU (sensor) frame -> vehicle frame.
         *
         * Inputs
         *   x_mean, y_mean, z_mean : average accelerometer reading while the vehicle
         *                            is stationary.
         *   xy_ratio               : y/x ratio as the car accelerates straight.
         *   *_bias                 : manufacturer offsets, subtracted from the means first.
         *
         * Idea
         * 1: roll correction, rotate about x by alpha to zero the y component
         *     alpha = atan2(gy, gz)
         *     Rx(alpha) * g_s = (gx, 0, h),   where h = hypot(gy, gz)
         *
         * 2: pitch correction, rotate about y by beta to zero the x component
         *     beta  = -atan2(gx, h)
         *     Ry(beta) * (gx, 0, h) = (0, 0, |g|)
         *
         * 3: yaw correction, rotate about z by gamma to align the horizontal
         *     reference direction with the vehicle's x axis
         *     gamma = -atan(xy_ratio)
         *     (does not affect the z axis, so gravity alignment from steps 1-2 remains)
         *
         * Combined (apply x-rotation first, z-rotation last):
         *
         *     R = Rz(gamma) * Ry(beta) * Rx(alpha)
         *
         *         | 1    0     0   |        |  cb  0  sb |        | cg  -sg  0 |
         *   Rx =  | 0   ca   -sa   |  Ry =  |  0   1  0  |  Rz =  | sg   cg  0 |
         *         | 0   sa    ca   |        | -sb  0  cb |        | 0    0   1 |
         *
         *   (ca = cos(alpha), sa = sin(alpha), etc.)
         *
         * R is a pure rotation (orthonormal, det = +1), so its inverse is its
         * transpose and it preserves vector magnitudes.
         */
    }
};

struct ImuReadings
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

ImuReadings apply(ImuReorient &reorient, ImuReadings &imu);
} // namespace app::reorient