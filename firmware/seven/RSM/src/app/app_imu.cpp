#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"

#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include <cmath>

using namespace io::imus;

namespace app::imu
{
static RSMImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                     .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                     .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                     .gyro_z_res  = std::unexpected(ErrorCode::ERROR),
                                    };

static constexpr float accel_limit_g = 5.0f;
static constexpr float gyro_limit_degs = 250.0f;

/// @brief Checks if imu reading is a valid number(isfinite) and if it is within a limit
/// @param imu_val 
/// @param limit 
/// @return 
static result<float> validate_val(const result<float> imu_val, const float limit)
{
    if (!imu_val.has_value()) {
        return imu_val;
    }
    else if(!std::isfinite(*imu_val)) {
        return std::unexpected(ErrorCode::INVALID_READING);
    }
    else if(std::fabs(*imu_val) > limit) {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }
    else {
        return imu_val;
    }
}

void init()
{
    auto ec = io::imus::init();
    can_alerts::infos::ImuInitFailed_set(not ec.has_value());
}

void broadcast()
{
    imu_results = { validate_val(imu_rear.getAccelX(), accel_limit_g),
                    validate_val(imu_rear.getAccelY(), accel_limit_g),
                    validate_val(imu_rear.getAccelZ(), accel_limit_g),
                    validate_val(imu_rear.getGyroX(),  gyro_limit_degs),
                    validate_val(imu_rear.getGyroY(),  gyro_limit_degs),
                    validate_val(imu_rear.getGyroZ(),  gyro_limit_degs) };

    can_tx::RSM_AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::RSM_AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::RSM_AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::RSM_GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::RSM_GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::RSM_GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));
    can_alerts::infos::ImuDataBad_set(imu_results.hasFault());
}
}