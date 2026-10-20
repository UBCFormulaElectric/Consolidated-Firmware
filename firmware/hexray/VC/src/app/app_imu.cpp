#include <algorithm>
#include <cmath>
#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imus
{
using namespace app::can_alerts;
using namespace io::imus;
using namespace std;

static VCImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                    .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

result<void> imu1_init_ok = std::unexpected(ErrorCode::ERROR_INDETERMINATE);

void init()
{
    imu1_init_ok = IMU1.init();
    infos::Imu1InitFailed_set(not imu1_init_ok.has_value());
}

namespace
{
    // Hard coded values taken from manually sampling
    // These are dummy values for now. In future, sample during boot
    constexpr float mean_x_reading = 0.2f;
    constexpr float mean_y_reading = 0.1f;
    constexpr float mean_z_reading = 0.97468f;

    // TODO Find biases by laying IMU flat
    constexpr float x_bias = 0.0f;
    constexpr float y_bias = 0.0f;
    constexpr float z_bias = 0.0f;

    // Bias corrected values
    constexpr float corrected_x = mean_x_reading - x_bias;
    constexpr float corrected_y = mean_y_reading - y_bias;
    constexpr float corrected_z = mean_z_reading - z_bias;

    // TODO: Ratio needs to be found during driving
    constexpr float xy_ratio = 0.0f; // mean y/x when moving straight

    const float alpha = atan2(corrected_y, corrected_z);                      // angle to rotate by about x axis
    const float beta  = -atan2(corrected_x, hypot(corrected_y, corrected_z)); // angle to rotate by about y axis
    const float gamma = -atan(xy_ratio);

    const float cos_alpha = cos(alpha);
    const float cos_beta  = cos(beta);
    const float cos_gamma = cos(gamma);
    const float sin_alpha = sin(alpha);
    const float sin_beta  = sin(beta);
    const float sin_gamma = sin(gamma);

    static const array<array<float, 3>, 3> R = {
        { { { cos_beta * cos_gamma, sin_alpha *sin_beta *cos_gamma - cos_alpha *sin_gamma,
              cos_alpha *sin_beta *cos_gamma + sin_alpha *sin_gamma } },
          { { cos_beta * sin_gamma, sin_alpha *sin_beta *sin_gamma + cos_alpha *cos_gamma,
              cos_alpha *sin_beta *sin_gamma - sin_alpha *cos_gamma } },
          { { -sin_beta, sin_alpha *cos_beta, cos_alpha *cos_beta } } }
    }; // rotation matrix R = Rz(gamma) * Ry(beta) * Rx(alpha)

    void reorient_imu(
        result<float> accelX,
        result<float> accelY,
        result<float> accelZ,
        result<float> gyroX,
        result<float> gyroY,
        result<float> gyroZ)
    {
        if (accelX.has_value() && accelY.has_value() && accelZ.has_value())
        {
            float x                 = accelX.value();
            float y                 = accelY.value();
            float z                 = accelZ.value();
            imu_results.accel_x_res = R[0][0] * x + R[0][1] * y + R[0][2] * z;
            imu_results.accel_y_res = R[1][0] * x + R[1][1] * y + R[1][2] * z;
            imu_results.accel_z_res = R[2][0] * x + R[2][1] * y + R[2][2] * z;
        }
        else
        {
            imu_results.accel_x_res = std::unexpected(ErrorCode::ERROR);
            imu_results.accel_y_res = std::unexpected(ErrorCode::ERROR);
            imu_results.accel_z_res = std::unexpected(ErrorCode::ERROR);
        }

        if (gyroX.has_value() && gyroY.has_value() && gyroZ.has_value())
        {
            float x                = gyroX.value();
            float y                = gyroY.value();
            float z                = gyroZ.value();
            imu_results.gyro_x_res = R[0][0] * x + R[0][1] * y + R[0][2] * z;
            imu_results.gyro_y_res = R[1][0] * x + R[1][1] * y + R[1][2] * z;
            imu_results.gyro_z_res = R[2][0] * x + R[2][1] * y + R[2][2] * z;
        }
        else
        {
            imu_results.gyro_x_res = std::unexpected(ErrorCode::ERROR);
            imu_results.gyro_y_res = std::unexpected(ErrorCode::ERROR);
            imu_results.gyro_z_res = std::unexpected(ErrorCode::ERROR);
        }
    }

} // namespace

void broadcast()
{
    reorient_imu(
        IMU1.getAccelX(), IMU1.getAccelY(), IMU1.getAccelZ(), IMU1.getGyroX(), IMU1.getGyroY(), IMU1.getGyroZ());

    can_tx::VC_Imu1AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::VC_Imu1AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::VC_Imu1AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::VC_Imu1GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::VC_Imu1GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::VC_Imu1GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));
}

result<float> getAccelX()
{
    return imu_results.accel_x_res;
}

result<float> getAccelY()
{
    return imu_results.accel_y_res;
}

result<float> getAccelZ()
{
    return imu_results.accel_z_res;
}

result<float> getGyroX()
{
    return imu_results.gyro_x_res;
}

result<float> getGyroY()
{
    return imu_results.gyro_y_res;
}

result<float> getGyroZ()
{
    return imu_results.gyro_z_res;
}

bool initSuccess()
{
    return imu1_init_ok.has_value();
}
} // namespace app::imus
