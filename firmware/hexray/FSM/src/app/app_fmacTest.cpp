#include "app_fmacTest.hpp"
#include "hw_fmac.hpp"
#include "io_imus.hpp"
#include "app_canTx.hpp"
#include "main.h"

extern "C" FMAC_HandleTypeDef hfmac;

namespace app::fmac_test
{

/*
 * First-order IIR low-pass filter for yaw rate (gyro Z).
 *
 * Design: bilinear-transformed RC, fs = 100 Hz, fc = 10 Hz.
 *   alpha = 2*pi*fc/fs / (1 + 2*pi*fc/fs) ≈ 0.3859
 *
 * FMAC IIR Direct Form 1 (P=2 b-taps min, Q=1 a-tap):
 *   y[n] = 2^(-R) * { b0*x[n] + b1*x[n-1] + a1*y[n-1] }
 *
 * All coefficients in Q1.15 format (multiply float by 32768).
 */
static constexpr int16_t kCoefB[] = { 12644, 0 }; // b0 ≈ 0.3859,  b1 = 0
static constexpr int16_t kCoefA[] = { 20125 };    // a1 ≈ 0.6141
static constexpr uint8_t kGainExp = 0;

static hw::fmac::FmacIir yaw_filter(hfmac);

void init()
{
    auto result = yaw_filter.init(kCoefB, kCoefA, kGainExp);
    if (!result)
        LOG_ERROR("FMAC yaw filter init failed: %d", static_cast<int>(result.error()));
}

void broadcast()
{
    const auto raw_result = io::imus::imu_front.getGyroZ();
    if (!raw_result.has_value())
        return;

    const float raw      = raw_result.value();
    float       filtered = 0.0f;
    const auto  err      = yaw_filter.process(raw, &filtered);
    if (err != ErrorCode::OK)
        return;

    can_tx::FSM_RawYawRate_set(raw);
    can_tx::FSM_FilteredYawRate_set(filtered);
}

} // namespace app::fmac_test
