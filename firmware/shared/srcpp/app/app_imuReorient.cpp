#include "app_imuReorient.hpp"
#include "util_errorCodes.hpp"
#include <cmath>
#include <array>

using namespace std;
namespace app::reorient
{
/*
 * Converts raw IMU readings into the vehicle frame.
 * See app_imuReorient.hpp for more details
 *
 * For each 3-axis sensor (accelerometer and gyro):
 *     v_vehicle = R * (v_raw - bias)
 *
 * Bias is removed first, in the sensor frame. The same R is applied to both
 * sensors since they share a mounting. If any channel has a fault, every
 * output channel is returned as an error rather than a partially valid result.
 */
ImuReadings apply(ImuReorient &reorient, ImuReadings &imu)
{
    if (imu.hasFault())
    {
        return {
            .accelX = std::unexpected(ErrorCode::ERROR),
            .accelY = std::unexpected(ErrorCode::ERROR),
            .accelZ = std::unexpected(ErrorCode::ERROR),
            .gyroX  = std::unexpected(ErrorCode::ERROR),
            .gyroY  = std::unexpected(ErrorCode::ERROR),
            .gyroZ  = std::unexpected(ErrorCode::ERROR),
        };
    }

    const float x     = imu.accelX.value() - reorient.x_bias;
    const float y     = imu.accelY.value() - reorient.y_bias;
    const float z     = imu.accelZ.value() - reorient.z_bias;
    const float gyroX = imu.gyroX.value() - reorient.roll_bias;
    const float gyroY = imu.gyroY.value() - reorient.pitch_bias;
    const float gyroZ = imu.gyroZ.value() - reorient.yaw_bias;

    return {
        .accelX = reorient.R[0][0] * x + reorient.R[0][1] * y + reorient.R[0][2] * z,
        .accelY = reorient.R[1][0] * x + reorient.R[1][1] * y + reorient.R[1][2] * z,
        .accelZ = reorient.R[2][0] * x + reorient.R[2][1] * y + reorient.R[2][2] * z,
        .gyroX  = reorient.R[0][0] * gyroX + reorient.R[0][1] * gyroY + reorient.R[0][2] * gyroZ,
        .gyroY  = reorient.R[1][0] * gyroX + reorient.R[1][1] * gyroY + reorient.R[1][2] * gyroZ,
        .gyroZ  = reorient.R[2][0] * gyroX + reorient.R[2][1] * gyroY + reorient.R[2][2] * gyroZ,
    };
}
} // namespace app::reorient
