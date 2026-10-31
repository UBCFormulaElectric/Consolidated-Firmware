#include "app_imuReorient.hpp"
#include "util_errorCodes.hpp"
#include <cmath>
#include <array>

using namespace std;
namespace app::reorient
{
imu_readings apply(imu_reorient &reorient, imu_readings &imu)
{
    imu_readings imu_results;
    if (!imu.hasFault())
    {
        const float x     = imu.accelX.value() - reorient.x_bias;
        const float y     = imu.accelY.value() - reorient.y_bias;
        const float z     = imu.accelZ.value() - reorient.z_bias;
        const float gyroX = imu.gyroX.value() - reorient.roll_bias;
        const float gyroY = imu.gyroY.value() - reorient.pitch_bias;
        const float gyroZ = imu.gyroZ.value() - reorient.yaw_bias;

        imu_results.accelX = reorient.R[0][0] * x + reorient.R[0][1] * y + reorient.R[0][2] * z;
        imu_results.accelY = reorient.R[1][0] * x + reorient.R[1][1] * y + reorient.R[1][2] * z;
        imu_results.accelZ = reorient.R[2][0] * x + reorient.R[2][1] * y + reorient.R[2][2] * z;
        imu_results.gyroX  = reorient.R[0][0] * gyroX + reorient.R[0][1] * gyroY + reorient.R[0][2] * gyroZ;
        imu_results.gyroY  = reorient.R[1][0] * gyroX + reorient.R[1][1] * gyroY + reorient.R[1][2] * gyroZ;
        imu_results.gyroZ  = reorient.R[2][0] * gyroX + reorient.R[2][1] * gyroY + reorient.R[2][2] * gyroZ;
    }
    else
    {
        imu_results.accelX = std::unexpected(ErrorCode::ERROR);
        imu_results.accelY = std::unexpected(ErrorCode::ERROR);
        imu_results.accelZ = std::unexpected(ErrorCode::ERROR);
        imu_results.gyroX  = std::unexpected(ErrorCode::ERROR);
        imu_results.gyroY  = std::unexpected(ErrorCode::ERROR);
        imu_results.gyroZ  = std::unexpected(ErrorCode::ERROR);
    }
    return imu_results;
}
} // namespace app::reorient
