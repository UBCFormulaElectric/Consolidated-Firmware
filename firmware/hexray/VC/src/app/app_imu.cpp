#include <algorithm>
#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imus
{
using namespace app::can_alerts;
using namespace io::imus;

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

void broadcast()
{
    // IMU1 data broadcasting
    imu_results = { IMU1.getAccelX(), IMU1.getAccelY(), IMU1.getAccelZ(),
                    IMU1.getGyroX(),  IMU1.getGyroY(),  IMU1.getGyroZ() };

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
