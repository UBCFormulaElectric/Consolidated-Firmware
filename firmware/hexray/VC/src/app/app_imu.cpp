#include <algorithm>
#include <array>
#include <cmath>

#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"
#include "util_units.hpp"

#ifdef TARGET_EMBEDDED
#include "hw_fmac.hpp"
#include "main.h"
#endif

namespace app::imus
{
using namespace app::can_alerts;

namespace
{
    constexpr float kImuTaskRateHz     = 100.0f;
    constexpr float kImu1GyroZCutoffHz = 10.0f;
    // Gyro full-scale (matches GYRO_DPS_250 in io_imu.cpp); used to map deg/s to the FMAC's Q1.15 ±1.0.
    constexpr float kGyroFullScaleDps = 250.0f;

#ifdef TARGET_EMBEDDED
    constexpr int16_t floatToQ15(float x)
    {
        return static_cast<int16_t>(x * 32768.0f);
    }

    struct FirstOrderButterworthQ15
    {
        std::array<int16_t, 2> b;
        std::array<int16_t, 1> a;
    };

    FirstOrderButterworthQ15 butterworthQ15(float cutoff_frequency, float sample_rate)
    {
        const float tan_arg                 = M_PI_F * cutoff_frequency / sample_rate;
        const float gamma                   = 1.0f / tanf(tan_arg);
        const float one_over_one_plus_gamma = 1.0f / (1.0f + gamma);
        const float b0                      = one_over_one_plus_gamma;
        const float b1                      = b0;
        const float a1                      = (1.0f - gamma) * one_over_one_plus_gamma;

        FirstOrderButterworthQ15 coefs{};
        coefs.b = { { floatToQ15(b0), floatToQ15(b1) } };
        // FMAC pCoeffA is the y[n-1] tap weight in y = b0*x + b1*x[n-1] + A1*y[n-1] (ST AN5305).
        // Our Butterworth form stores a1 in y = ... - a1*y[n-1], so pass -a1.
        coefs.a = { { floatToQ15(-a1) } };
        return coefs;
    }

    static hw::fmac::FmacIir imu1_gyro_z_fmac{ hfmac };
#endif
} // namespace

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
    warnings::Imu1InitFailed_set(not imu1_init_ok.has_value());

#ifdef TARGET_EMBEDDED
    if (imu1_init_ok)
    {
        const auto coefs = butterworthQ15(kImu1GyroZCutoffHz, kImuTaskRateHz);
        LOG_IF_ERR(imu1_gyro_z_fmac.init(coefs.b, coefs.a, 0U));
    }
#endif
}

void broadcast()
{
    imu_results = { IMU1.getAccelX(), IMU1.getAccelY(), IMU1.getAccelZ(),
                    IMU1.getGyroX(),  IMU1.getGyroY(),  IMU1.getGyroZ() };

    // Filter gyro Z with the FMAC; only run it on a valid reading so a failed SPI transaction
    // doesn't push garbage into the filter state.
#ifdef TARGET_EMBEDDED
    result<float> filtered_gyro_z = imu_results.gyro_z_res;
    if (imu_results.gyro_z_res)
    {
        const auto filtered = imu1_gyro_z_fmac.process(imu_results.gyro_z_res.value(), kGyroFullScaleDps);
        if (filtered)
            filtered_gyro_z = filtered.value();
        else
            LOG_IF_ERR(filtered);
    }
#else
    const result<float> &filtered_gyro_z = imu_results.gyro_z_res;
#endif

    can_tx::VC_Imu1AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::VC_Imu1AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::VC_Imu1AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::VC_Imu1GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::VC_Imu1GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::VC_Imu1GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));
    can_tx::VC_Imu1GyroZFiltered_set(filtered_gyro_z.value_or(0.0f));
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
