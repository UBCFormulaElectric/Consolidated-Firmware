#include "hw_fmac.hpp"
#include "main.h"
#include "hw_utils.hpp"
#include <cassert>

namespace hw::fmac
{

namespace
{
    constexpr int16_t floatToQ1Point5(float x) { return static_cast<int16_t>(x * 32768.0f); }
    constexpr float   q1Point5ToFloat(int16_t x) { return static_cast<float>(x) / 32768.0f; }

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

std::expected<void, ErrorCode> FmacIir::init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent)
{
    const auto numBTaps = static_cast<uint8_t>(coefB.size());
    const auto numATaps = static_cast<uint8_t>(coefA.size());

    assert(numBTaps >= IIR_MIN_B_TAPS && numBTaps <= IIR_MAX_B_TAPS);
    assert(numATaps >= IIR_MIN_A_TAPS && numATaps <= IIR_MAX_A_TAPS);
    assert(gainExponent >= IIR_MIN_GAIN && gainExponent <= IIR_MAX_GAIN);

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
        .pCoeffA           = const_cast<int16_t *>(coefA.data()),
        .CoeffASize        = numATaps,
        .pCoeffB           = const_cast<int16_t *>(coefB.data()),
        .CoeffBSize        = numBTaps,
        .InputAccess       = FMAC_BUFFER_ACCESS_POLLING,
        .OutputAccess      = FMAC_BUFFER_ACCESS_POLLING,
        .Clip              = FMAC_CLIP_ENABLED,
        .Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1,
        .P                 = numBTaps,
        .Q                 = numATaps,
        .R                 = gainExponent,
    };

    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_FMAC_FilterConfig(&m_handle, &filterConfig)));
    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_FMAC_FilterPreload(
        &m_handle, const_cast<int16_t *>(coefB.data()), numBTaps, const_cast<int16_t *>(coefA.data()), numATaps)));
    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_FMAC_FilterStart(&m_handle, &m_outputSample, &m_outputSize)));
    return {};
}

std::expected<void, ErrorCode> FmacIir::pushSample(int16_t sample_q15)
{
    uint16_t size = 1;
    return hw_utils_convertHalStatus(HAL_FMAC_AppendFilterData(&m_handle, &sample_q15, &size));
}

std::expected<int16_t, ErrorCode> FmacIir::popSample()
{
    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_FMAC_PollFilterData(&m_handle, kPollFilterDataTimeoutMs)));
    return m_outputSample;
}

ErrorCode FmacIir::process(const float sample, float *output)
{
    auto err = pushSample(floatToQ1Point5(sample));
    if (!err)
        return err.error();
    auto out_q15 = popSample();
    if (!out_q15)
        return out_q15.error();
    *output = q1Point5ToFloat(out_q15.value());
    return ErrorCode::OK;
}

ErrorCode FmacIir::stop()
{
    auto result = hw_utils_convertHalStatus(HAL_FMAC_FilterStop(&m_handle));
    return result ? ErrorCode::OK : result.error();
}

} // namespace hw::fmac
