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
    imu_results = { imu_front.getAccelX(), imu_front.getAccelY(), imu_front.getAccelZ(),
                    imu_front.getGyroX(),  imu_front.getGyroY(),  imu_front.getGyroZ() };

    can_tx::FSM_AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::FSM_AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::FSM_AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::FSM_GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::FSM_GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::FSM_GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));

    accel_x = imu_faults.accel_x_fault ? accel_x : 0.0f;
    accel_y = imu_faults.accel_y_fault ? accel_y : 0.0f;
    accel_z = imu_faults.accel_z_fault ? accel_z : 0.0f;
    gyro_x  = imu_faults.gyro_x_fault ? gyro_x : 0.0f;
    gyro_y  = imu_faults.gyro_y_fault ? gyro_y : 0.0f;
    gyro_z  = imu_faults.gyro_z_fault ? gyro_z : 0.0f;

    io::Imu::AccelData imu_accel_data = { accel_x, accel_y, accel_z };
    io::Imu::GyroData  imu_gyro_data  = { gyro_x, gyro_y, gyro_z };

    app_canTx_FSM_LinearAccelerationInX_set(imu_accel_data.x);
    app_canTx_FSM_LinearAccelerationInY_set(imu_accel_data.y);
    app_canTx_FSM_LinearAccelerationInZ_set(imu_accel_data.z);
    app_canTx_FSM_RollAngularAcceleration_set(imu_gyro_data.x);
    app_canTx_FSM_PitchAngularAcceleration_set(imu_gyro_data.y);
    app_canTx_FSM_YawAngularAcceleration_set(imu_gyro_data.z);
#endif // TARGET_TEST
}
} // namespace app::imu
