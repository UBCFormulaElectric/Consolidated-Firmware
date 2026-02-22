#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"

#ifdef TARGET_TEST // TODO make proper fakes
const io::imu::Imu imu_config;
#endif // TARGET_TEST
namespace app::imu
{
static FSMImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                     .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

void broadcast()
{
    imu_results = {
        imu_config.getAccelX(), imu_config.getAccelY(), imu_config.getAccelZ(),
        imu_config.getGyroX(),  imu_config.getGyroY(),  imu_config.getGyroZ(),
    };
    can_tx::FSM_LinearAccelerationInX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::FSM_LinearAccelerationInY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::FSM_LinearAccelerationInZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::FSM_RollAngularAcceleration_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::FSM_PitchAngularAcceleration_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::FSM_YawAngularAcceleration_set(imu_results.gyro_z_res.value_or(0.0f));
}
} // namespace app::imu
