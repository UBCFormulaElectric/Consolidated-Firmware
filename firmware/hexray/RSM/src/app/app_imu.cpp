#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"
<<<<<<< HEAD
#include "app_canTx.hpp"

#ifdef TARGET_TEST
const io::imu::Imu imu_config;
#endif // TARGET_TEST
=======

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

using namespace io::imus;
>>>>>>> 813a60e32 (IMU test set up for RSM)
namespace app::imu
{
static RSMImuFaults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                    .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

void init()
{
    auto ec = imu_r.init();
    app_canAlerts_RSM_Info_ImuInitFailed_set(not ec.has_value());
}

void broadcast()
{
    io::Imu::AccelData accel{};
    io::Imu::GyroData gyro{};

    imu_faults.accel_x_fault = imu_r.getAccelX(accel.x);
    imu_faults.accel_y_fault = imu_r.getAccelY(accel.y);
    imu_faults.accel_z_fault = imu_r.getAccelZ(accel.z);
    imu_faults.gyro_x_fault  = imu_r.getGyroX(gyro.x);
    imu_faults.gyro_y_fault  = imu_r.getGyroY(gyro.y);
    imu_faults.gyro_z_fault  = imu_r.getGyroZ(gyro.z);

    accel.x = imu_faults.accel_x_fault.has_value() ? accel.x : 0.0f;
    accel.y = imu_faults.accel_y_fault.has_value() ? accel.y : 0.0f;
    accel.z = imu_faults.accel_z_fault.has_value() ? accel.z : 0.0f;
    gyro.x  = imu_faults.gyro_x_fault.has_value() ? gyro.x : 0.0f;
    gyro.y  = imu_faults.gyro_y_fault.has_value() ? gyro.y : 0.0f;
    gyro.z  = imu_faults.gyro_z_fault.has_value() ? gyro.z : 0.0f;

    app_canTx_RSM_AccelX_set(accel.x);
    app_canTx_RSM_AccelY_set(accel.y);
    app_canTx_RSM_AccelZ_set(accel.z);
    app_canTx_RSM_GyroX_set(gyro.x);
    app_canTx_RSM_GyroY_set(gyro.y);
    app_canTx_RSM_GyroZ_set(gyro.z);

    app_canAlerts_RSM_Info_ImuDataBad_set(imu_faults.hasFault());
}
} // namespace app::imu
