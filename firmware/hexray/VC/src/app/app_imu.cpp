#include <algorithm>
#include <cmath>
#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"
#include "app_imuReorient.hpp"

namespace app::imus
{
using namespace app::can_alerts;
using namespace io::imus;
using namespace std;
using namespace app::reorient;

static imu_readings imu_results = { .accelX = std::unexpected(ErrorCode::ERROR),
                                    .accelY = std::unexpected(ErrorCode::ERROR),
                                    .accelZ = std::unexpected(ErrorCode::ERROR),
                                    .gyroX  = std::unexpected(ErrorCode::ERROR),
                                    .gyroY  = std::unexpected(ErrorCode::ERROR),
                                    .gyroZ  = std::unexpected(ErrorCode::ERROR) };

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
    // TODO: sample when VC on car 
    constexpr float mean_x_reading = 0.0f;
    constexpr float mean_y_reading = 0.0f;
    constexpr float mean_z_reading = 1.0f;

    // TODO: Use better setup to lay IMU flat
    constexpr float x_bias     = 0.04f;
    constexpr float y_bias     = 0.01f;
    constexpr float z_bias     = -0.01f;
    constexpr float roll_bias  = 0.63f;
    constexpr float pitch_bias = -0.4f;
    constexpr float yaw_bias   = -0.26f;

    // TODO: Ratio needs to be found during driving
    constexpr float xy_ratio = 0.0f; // mean y/x when moving straight

    imu_reorient reorient(
        x_bias_in,
        y_bias,
        z_bias,
        roll_bias,
        pitch_bias,
        yaw_bias,
        mean_x_reading,
        mean_y_reading,
        mean_z_reading,
        xy_ratio);
} // namespace

void broadcast()
{
    imu_readings raw = {
        IMU1.getAccelX(), IMU1.getAccelY(), IMU1.getAccelZ(), IMU1.getGyroX(), IMU1.getGyroY(), IMU1.getGyroZ(),
    };

    const imu_readings out = apply(reorient, raw);

    can_tx::VC_Imu1AccelX_set(out.accelX.value_or(0.0f));
    can_tx::VC_Imu1AccelY_set(out.accelY.value_or(0.0f));
    can_tx::VC_Imu1AccelZ_set(out.accelZ.value_or(0.0f));
    can_tx::VC_Imu1GyroX_set(out.gyroX.value_or(0.0f));
    can_tx::VC_Imu1GyroY_set(out.gyroY.value_or(0.0f));
    can_tx::VC_Imu1GyroZ_set(out.gyroZ.value_or(0.0f));
}

result<float> getAccelX()
{
    return imu_results.accelX;
}

result<float> getAccelY()
{
    return imu_results.accelY;
}

result<float> getAccelZ()
{
    return imu_results.accelZ;
}

result<float> getGyroX()
{
    return imu_results.gyroX;
}

result<float> getGyroY()
{
    return imu_results.gyroY;
}

result<float> getGyroZ()
{
    return imu_results.gyroZ;
}

bool initSuccess()
{
    return imu1_init_ok.has_value();
}
} // namespace app::imus
