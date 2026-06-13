#include "hw_fmac.hpp"

#include "main.h"
#ifndef HAL_FMAC_MODULE_ENABLED
#error "HAL_FMAC_MODULE_ENABLED must be defined and set to 1"
#endif

#include "hw_utils.hpp"
#include <algorithm>
#include <cassert>

namespace hw::fmac
{

namespace
{
    constexpr int16_t floatToQ1Point5(float x)
    {
        return static_cast<int16_t>(x * 32768.0f);
    }
    constexpr float q1Point5ToFloat(int16_t x)
    {
        return static_cast<float>(x) / 32768.0f;
    }

    // HAL_MAX_DELAY is 0xFFFFFFFF (wait forever). Use a finite timeout (ms) so we can tune on hardware.
    inline constexpr uint32_t kPollFilterDataTimeoutMs = 100;

    // IIR filter limits from FMAC reference manual (RM0481)
    constexpr uint8_t IIR_MIN_B_TAPS = 2;  // P min
    constexpr uint8_t IIR_MAX_B_TAPS = 64; // P max
    constexpr uint8_t IIR_MIN_A_TAPS = 1;  // Q min
    constexpr uint8_t IIR_MAX_A_TAPS = 63; // Q max
    constexpr uint8_t IIR_MIN_GAIN   = 0;  // R min (gain = 2^R)
    constexpr uint8_t IIR_MAX_GAIN   = 7;  // R max
} // namespace

result<void> FmacIir::init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent)
{
    const auto numBTaps = static_cast<uint8_t>(coefB.size());
    const auto numATaps = static_cast<uint8_t>(coefA.size());

    assert(numBTaps >= IIR_MIN_B_TAPS && numBTaps <= IIR_MAX_B_TAPS);
    assert(numATaps >= IIR_MIN_A_TAPS && numATaps <= IIR_MAX_A_TAPS);
    assert(gainExponent >= IIR_MIN_GAIN && gainExponent <= IIR_MAX_GAIN);

    // Store the config so armFilter() can re-arm the filter without the caller's coefficient buffers,
    // which are typically temporaries that don't outlive this call.
    std::copy(coefB.begin(), coefB.end(), m_coefB.begin());
    std::copy(coefA.begin(), coefA.end(), m_coefA.begin());
    m_numBTaps     = numBTaps;
    m_numATaps     = numATaps;
    m_gainExponent = gainExponent;

    return armFilter();
}

result<void> FmacIir::armFilter()
{
    const uint8_t numBTaps = m_numBTaps;
    const uint8_t numATaps = m_numATaps;

    // FilterConfig requires START=0, so stop/reset first. Ignore the result: on the initial arm the
    // filter isn't running yet (FilterStop just resets the already-idle peripheral).
    (void)HAL_FMAC_FilterStop(&m_handle);

    const uint16_t x1size = numBTaps + 1;
    const uint16_t x2size = numATaps + numBTaps;
    const uint16_t ysize  = numATaps + 1;
    const uint16_t x1Base = 0;
    const uint16_t x2Base = x1size;
    const uint16_t yBase  = x2Base + x2size;

    assert(yBase + ysize <= 256);

    FMAC_FilterConfigTypeDef filterConfig = {
        .InputBaseAddress  = static_cast<uint8_t>(x1Base),
        .InputBufferSize   = static_cast<uint8_t>(x1size),
        .InputThreshold    = FMAC_THRESHOLD_1,
        .CoeffBaseAddress  = static_cast<uint8_t>(x2Base),
        .CoeffBufferSize   = static_cast<uint8_t>(x2size),
        .OutputBaseAddress = static_cast<uint8_t>(yBase),
        .OutputBufferSize  = static_cast<uint8_t>(ysize),
        .OutputThreshold   = FMAC_THRESHOLD_1,
        .pCoeffA           = m_coefA.data(),
        .CoeffASize        = numATaps,
        .pCoeffB           = m_coefB.data(),
        .CoeffBSize        = numBTaps,
        .InputAccess       = FMAC_BUFFER_ACCESS_POLLING,
        .OutputAccess      = FMAC_BUFFER_ACCESS_POLLING,
        .Clip              = FMAC_CLIP_ENABLED,
        .Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1,
        .P                 = numBTaps,
        .Q                 = numATaps,
        .R                 = m_gainExponent,
    };

    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterConfig(&m_handle, &filterConfig)));

    // Prime the X1/Y delay lines with zero history (ST AN5305). FilterPreload must not receive coef arrays.
    // For zero initial conditions the input history is P-1 samples (x[n-1]..x[n-N]) and the output history
    // is Q samples (y[n-1]..y[n-M]); preloading P inputs would inject an extra zero into the pipeline.
    const uint8_t numInputHistory             = static_cast<uint8_t>(numBTaps - 1);
    int16_t       zero_input[IIR_MAX_B_TAPS]  = {};
    int16_t       zero_output[IIR_MAX_A_TAPS] = {};
    RETURN_IF_ERR(hw::utils::convertHalStatus(
        HAL_FMAC_FilterPreload(&m_handle, zero_input, numInputHistory, zero_output, numATaps)));

    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterStart(&m_handle, &m_outputSample, &m_outputSize)));
    return {};
}

result<void> FmacIir::pushSample(int16_t sample_q15)
{
    // m_inputSample/m_inputSize must stay alive until popSample()'s HAL_FMAC_PollFilterData consumes them:
    // HAL_FMAC_AppendFilterData only stores these pointers, the actual write happens during the poll.
    m_inputSample = sample_q15;
    m_inputSize   = 1;
    return hw::utils::convertHalStatus(HAL_FMAC_AppendFilterData(&m_handle, &m_inputSample, &m_inputSize));
}

result<int16_t> FmacIir::popSample()
{
    // PollFilterData clears pOutput after each call; re-register the output buffer every iteration.
    // First poll often completes input write only (OR exit); later polls drain the ready output sample.
    const uint32_t start = HAL_GetTick();
    do
    {
        m_outputSize = 1;
        RETURN_IF_ERR(
            hw::utils::convertHalStatus(HAL_FMAC_ConfigFilterOutputBuffer(&m_handle, &m_outputSample, &m_outputSize)));
        // PollFilterData returns HAL_ERROR on its per-call timeout when the next output sample isn't
        // ready yet. That's expected while the filter computes, so don't treat it as fatal: keep
        // polling until our own budget elapses and decide success purely on m_outputSize.
        (void)HAL_FMAC_PollFilterData(&m_handle, 1U);
        if (m_outputSize == 1U)
            return m_outputSample;
    } while ((HAL_GetTick() - start) < kPollFilterDataTimeoutMs);
    return std::unexpected(ErrorCode::TIMEOUT);
}

result<int16_t> FmacIir::processSample(const int16_t sample_q15)
{
    // If the append fails, the FMAC handshake has desynced (typically the START bit was cleared under
    // load). Re-arm the filter from the stored config and retry once before giving up on this sample.
    if (!pushSample(sample_q15))
    {
        RETURN_IF_ERR(armFilter());
        RETURN_IF_ERR(pushSample(sample_q15));
    }
    return popSample();
}

result<float> FmacIir::process(const float sample, const float full_scale)
{
    // Q1.15 represents [-1.0, +1.0); map the physical value to that range using the sensor full-scale
    // (e.g. ±4 g for accel, ±250 deg/s for gyro), filter, then map back.
    // Clamp before the int16 cast: at +full-scale, sample/full_scale == 1.0 and 1.0f*32768 == 32768,
    // which overflows int16_t and wraps to -32768 (sign-flipped garbage exactly when the sensor is maxed out).
    // Saturate to the largest representable Q1.15 value (0x7FFF) instead.
    constexpr float kMaxQ15Float = 32767.0f / 32768.0f;
    const float     normalized   = std::clamp(sample / full_scale, -1.0f, kMaxQ15Float);
    const auto      out_q15      = processSample(floatToQ1Point5(normalized));
    if (!out_q15)
        return std::unexpected(out_q15.error());
    return q1Point5ToFloat(out_q15.value()) * full_scale;
}

result<void> FmacIir::stop()
{
    return hw::utils::convertHalStatus(HAL_FMAC_FilterStop(&m_handle));
}

#ifdef HW_FMAC_DEBUG
// TEMPORARY (remove with the rest of the HW_FMAC_DEBUG instrumentation): snapshot of the HAL error code
// (HAL_FMAC_ERROR_* bitfield) and the FMAC status register (X1FULL/YEMPTY/OVFL/UNFL/SAT live flags).
FmacIir::DebugStatus FmacIir::debugStatus() const
{
    return DebugStatus{ m_handle.ErrorCode, READ_REG(m_handle.Instance->SR) };
}
#endif

} // namespace hw::fmac
