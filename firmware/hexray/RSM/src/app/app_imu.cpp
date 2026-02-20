#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"

#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

using namespace io::imus;
namespace app::imu
{
static RSMImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                    .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

void init()
{
    auto ec = imu_rear.init();
    can_alerts::infos::ImuInitFailed_set(!ec.has_value());
}

void broadcast()
{
    imu_results = { imu_rear.getAccelX(), imu_rear.getAccelY(), imu_rear.getAccelZ(),
                    imu_rear.getGyroX(),  imu_rear.getGyroY(),  imu_rear.getGyroZ() };

    can_tx::RSM_AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::RSM_AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::RSM_AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::RSM_GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::RSM_GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::RSM_GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));

    can_alerts::infos::ImuDataBad_set(imu_results.hasFault());
}
} // namespace app::imu
