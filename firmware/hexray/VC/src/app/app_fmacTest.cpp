#include "app_fmacTest.hpp"
#include "hw_fmac.hpp"
#include "io_imus.hpp"
#include "app_canTx.hpp"
#include "io_log.hpp"
#include <cmath>
#ifdef TARGET_EMBEDDED
#include "main.h"
extern "C" FMAC_HandleTypeDef hfmac;
#endif

namespace app::fmac_test
{

/*
 * First-order IIR low-pass filter for yaw rate (gyro Z).
 *
 * Design: bilinear-transformed RC, fs = 100 Hz, fc = 10 Hz.
 *   alpha = 2*pi*fc/fs / (1 + 2*pi*fc/fs) = 0.3859
 *
 * FMAC IIR Direct Form 1 (P=2 b-taps min, Q=1 a-tap):
 *   y[n] = 2^(-R) * { b0*x[n] + b1*x[n-1] + a1*y[n-1] }
 *
 * All coefficients in Q1.15 format (multiply float by 32768).
 */
static constexpr int16_t kCoefB[] = { 12644, 0 }; // b0 ≈ 0.3859,  b1 = 0
static constexpr int16_t kCoefA[] = { 20125 };    // a1 ≈ 0.6141
static constexpr uint8_t kGainExp = 0;

#ifdef TARGET_EMBEDDED
static hw::fmac::FmacIir yaw_filter(hfmac);
#else
static hw::fmac::FmacIir yaw_filter;
#endif
static bool fmac_ok = false;

// Synthetic 2 Hz square wave toggling between +5 and -5 deg/s.
// At 100 Hz broadcast rate, half-period = 25 ticks.
static constexpr float    kTestAmplitude = 5.0f;
static constexpr uint32_t kHalfPeriod    = 25;
static uint32_t           tick_count     = 0;

void init()
{
    auto result = yaw_filter.init(kCoefB, kCoefA, kGainExp);
    if (!result)
        LOG_ERROR("FMAC yaw filter init failed: %d", static_cast<int>(result.error()));
    else
        fmac_ok = true;
}

void broadcast()
{
    // Use IMU if available, otherwise fall back to synthetic square wave.
    float      raw;
    const auto raw_result = IMU1.getGyroZ();
    if (raw_result.has_value())
    {
        raw = raw_result.value();
    }
    else
    {
        raw = ((tick_count / kHalfPeriod) % 2 == 0) ? kTestAmplitude : -kTestAmplitude;
        ++tick_count;
    }

    float filtered = raw;
    if (fmac_ok)
    {
        if (!yaw_filter.process(raw, &filtered))
            filtered = raw;
    }

    app::can_tx::VC_RawYawRate_set(raw);
    app::can_tx::VC_FilteredYawRate_set(filtered);
}

} // namespace app::fmac_test
