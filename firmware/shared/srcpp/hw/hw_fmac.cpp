#include "hw_fmac.hpp"

#include "main.h"
#ifndef HAL_FMAC_MODULE_ENABLED
#error "HAL_FMAC_MODULE_ENABLED must be defined and set to 1"
#endif

#include "hw_utils.hpp"
#include <array>
#include <cassert>
#include <cmath>

namespace hw::fmac
{

namespace
{
    /** Round to nearest and saturate to [-1, 1); NaN maps to 0. */
    int16_t floatToQ15(const float x)
    {
        const float scaled = x * 32768.0f;
        if (std::isnan(scaled))
            return 0;
        if (scaled >= 32767.0f)
            return INT16_MAX;
        if (scaled <= -32768.0f)
            return INT16_MIN;
        return static_cast<int16_t>(std::lrint(scaled));
    }
    constexpr float q15ToFloat(const int16_t x)
    {
        return static_cast<float>(x) / 32768.0f;
    }

    // An output takes roughly P + Q FMAC clocks, so this only trips if the filter has stalled.
    constexpr uint32_t kMaxOutputPollIterations = 1000;

    // IIR filter limits from FMAC reference manual (RM0481)
    constexpr uint8_t IIR_MIN_B_TAPS = 2;  // P min
    constexpr uint8_t IIR_MAX_B_TAPS = 64; // P max
    constexpr uint8_t IIR_MIN_A_TAPS = 1;  // Q min
    constexpr uint8_t IIR_MAX_A_TAPS = 63; // Q max
    constexpr uint8_t IIR_MIN_GAIN   = 0;  // R min (gain = 2^R)
    constexpr uint8_t IIR_MAX_GAIN   = 7;  // R max

    constexpr std::array<int16_t, IIR_MAX_B_TAPS> kZeroHistory{};
} // namespace

result<void> FmacIir::init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent)
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

    // FilterConfig refuses to run while a filter is active; FilterStop also resets the buffer pointers and flags.
    if ((m_handle.Instance->PARAM & FMAC_PARAM_START) != 0U)
        RETURN_IF_ERR(stop());

    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterConfig(&m_handle, &filterConfig)));
    // Preload the history (not the coefficients, which FilterConfig already loaded into X2) with zeros:
    // P - 1 past inputs, so the first written sample completes a P-tap window and produces y[0], and Q past outputs.
    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterPreload(
        &m_handle, const_cast<int16_t *>(kZeroHistory.data()), static_cast<uint8_t>(numBTaps - 1),
        const_cast<int16_t *>(kZeroHistory.data()), numATaps)));
    // No output buffer is registered with the HAL; process() reads RDATA directly.
    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterStart(&m_handle, nullptr, nullptr)));
    return {};
}

result<float> FmacIir::process(const float sample)
{
    FMAC_TypeDef *const fmac = m_handle.Instance;
    if ((fmac->PARAM & FMAC_PARAM_START) == 0U)
        return std::unexpected(ErrorCode::ERROR);

    fmac->WDATA = static_cast<uint16_t>(floatToQ15(sample));
    for (uint32_t i = 0; i < kMaxOutputPollIterations; ++i)
    {
        if ((fmac->SR & FMAC_SR_YEMPTY) == 0U)
            return q15ToFloat(static_cast<int16_t>(fmac->RDATA & FMAC_RDATA_RDATA));
    }
    return std::unexpected(ErrorCode::TIMEOUT);
}

result<void> FmacIir::stop()
{
    return hw::utils::convertHalStatus(HAL_FMAC_FilterStop(&m_handle));
}

} // namespace hw::fmac
