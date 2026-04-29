#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"

#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

#ifdef TARGET_TEST
io::Imu imu_config;
#endif // TARGET_TEST
namespace app::imu
{
static FSMImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                     .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

void init()
{
    auto ec = io::imus::init();
    can_alerts::warnings::ImuInitFailed_set(not ec.has_value());
}

void broadcast()
{
    imu_results = { io::imus::imu_front.getAccelX(), io::imus::imu_front.getAccelY(), io::imus::imu_front.getAccelZ(),
                    io::imus::imu_front.getGyroX(),  io::imus::imu_front.getGyroY(),  io::imus::imu_front.getGyroZ() };

    can_tx::FSM_AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::FSM_AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::FSM_AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::FSM_GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::FSM_GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::FSM_GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));
}
} // namespace app::imu
