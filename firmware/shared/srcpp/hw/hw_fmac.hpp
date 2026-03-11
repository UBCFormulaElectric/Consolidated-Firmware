#pragma once

/**
 * @file hw_fmac.hpp
 * @brief C++ driver for the STM32 FMAC peripheral (IIR filter, polling mode).
 *
 * Reference documentation used for this driver:
 * - RM0481 (STM32H5 Reference manual on zotero): FMAC chapter — IIR formula, buffer layout (X1, X2, Y),
 *   sizes (P+ε, P+Q, Q+ε), q1.15 format, gain R, 256-word internal RAM.
 * - STM32H7 Filter Math Accelerator (FMAC) peripheral doc: 
 *   https://www.st.com/content/ccc/resource/training/technical/product_training/group1/b3/5b/b1/ac/e1/61/40/b7/STM32H7-Peripheral-Filter_Math_Accelerator_FMAC/files/STM32H7-Peripheral-Filter_Math_Accelerator_FMAC.pdf/_jcr_content/translations/en.STM32H7-Peripheral-Filter_Math_Accelerator_FMAC.pdf
 *   Same concepts; H5 HAL follows the same model.
 * - stm32h5xx_hal_fmac.h (STM32CubeH5): HAL API — HAL_FMAC_FilterConfig, HAL_FMAC_FilterPreload,
 *   HAL_FMAC_FilterStart, HAL_FMAC_AppendFilterData, HAL_FMAC_PollFilterData, HAL_FMAC_FilterStop.
 */

extern "C"
{
#include "hw_hal.hpp"
#ifndef HAL_FMAC_MODULE_ENABLED
#error "HAL_FMAC_MODULE_ENABLED must be defined and set to 1"
#endif
} // gracefully handle the case where the module is not enabled, but fail to compile if it is not defined. (Prevents
  // future linker errors if silently ignoring the error)

#include "hw_utils.hpp"
#include <array>
#include <cassert>

namespace hw::fmac
{

// IIR filter limits from FMAC reference manual (RM0481)
// P = feedforward (B) tap count, Q = feedback (A) tap count, R = output gain exponent
// Using english names for the variables to match the reference manual but increasing readability
constexpr uint8_t IIR_MIN_B_TAPS = 2;  // P min
constexpr uint8_t IIR_MAX_B_TAPS = 64; // P max
constexpr uint8_t IIR_MIN_A_TAPS = 1;  // Q min
constexpr uint8_t IIR_MAX_A_TAPS = 63; // Q max
constexpr uint8_t IIR_MIN_GAIN   = 0;  // R min (gain = 2^R)
constexpr uint8_t IIR_MAX_GAIN   = 7;  // R max

namespace detail
{
inline int16_t floatToQ15(float x) { return static_cast<int16_t>(x * 32768.0f); }
inline float   q15ToFloat(int16_t x) { return static_cast<float>(x) / 32768.0f; }

// HAL_MAX_DELAY is 0xFFFFFFFF (wait forever). Use a finite timeout (ms) so we can tune on hardware.
inline constexpr uint32_t kPollFilterDataTimeoutMs = 100;
} // namespace detail

template <size_t NUM_B_TAPS, size_t NUM_A_TAPS>
class FmacIir
{
    static_assert(NUM_B_TAPS >= IIR_MIN_B_TAPS && NUM_B_TAPS <= IIR_MAX_B_TAPS);
    static_assert(NUM_A_TAPS >= IIR_MIN_A_TAPS && NUM_A_TAPS <= IIR_MAX_A_TAPS);

    static constexpr uint16_t X1_SIZE = NUM_B_TAPS + 2;
    static constexpr uint16_t X2_SIZE = NUM_A_TAPS + NUM_B_TAPS;
    static constexpr uint16_t Y_SIZE  = NUM_A_TAPS + 2;
    static constexpr uint16_t X1_BASE = 0;
    static constexpr uint16_t X2_BASE = X1_SIZE;
    static constexpr uint16_t Y_BASE  = X2_BASE + X2_SIZE;
    static_assert(Y_BASE + Y_SIZE <= 256, "Coefficient/buffer layout exceeds 256-word FMAC RAM");

  public:
    FmacIir(
        FMAC_HandleTypeDef                    &handle,
        const std::array<int16_t, NUM_B_TAPS> &coefB,
        const std::array<int16_t, NUM_A_TAPS> &coefA,
        const uint8_t                          gainExponent)
      : m_handle(handle), m_coefB(coefB), m_coefA(coefA), m_gainExponent(gainExponent)
    {
        assert(gainExponent >= IIR_MIN_GAIN && gainExponent <= IIR_MAX_GAIN);
    }

    void init()
    {
        FMAC_FilterConfigTypeDef filterConfig = {
            .InputBaseAddress  = static_cast<uint8_t>(X1_BASE),
            .InputBufferSize   = static_cast<uint8_t>(X1_SIZE),
            .InputThreshold    = FMAC_THRESHOLD_1,
            .CoeffBaseAddress  = static_cast<uint8_t>(X2_BASE),
            .CoeffBufferSize   = static_cast<uint8_t>(X2_SIZE),
            .OutputBaseAddress = static_cast<uint8_t>(Y_BASE),
            .OutputBufferSize  = static_cast<uint8_t>(Y_SIZE),
            .OutputThreshold   = FMAC_THRESHOLD_1,
            .pCoeffB           = const_cast<int16_t *>(m_coefB.data()),
            .CoeffBSize        = static_cast<uint8_t>(NUM_B_TAPS),
            .pCoeffA           = const_cast<int16_t *>(m_coefA.data()),
            .CoeffASize        = static_cast<uint8_t>(NUM_A_TAPS),
            .InputAccess       = FMAC_BUFFER_ACCESS_POLLING,
            .OutputAccess      = FMAC_BUFFER_ACCESS_POLLING,
            .Clip              = FMAC_CLIP_ENABLED,
            .Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1,
            .P                 = static_cast<uint8_t>(NUM_B_TAPS),
            .Q                 = static_cast<uint8_t>(NUM_A_TAPS),
            .R                 = m_gainExponent,
        };

        assert(HAL_FMAC_FilterConfig(&m_handle, &filterConfig) == HAL_OK);
        assert(HAL_FMAC_FilterPreload(
                   &m_handle, const_cast<int16_t *>(m_coefB.data()), NUM_B_TAPS,
                   const_cast<int16_t *>(m_coefA.data()), NUM_A_TAPS) == HAL_OK);
        assert(HAL_FMAC_FilterStart(&m_handle, &m_outputSample, &m_outputSize) == HAL_OK);
    }

    /** Process one sample: float in -> FMAC (Q1.15) -> float out. */
    std::expected<float, ErrorCode> process(const float sample)
    {
        RETURN_IF_ERR(pushSample(detail::floatToQ15(sample)));
        const auto out_q15 = popSample();
        RETURN_IF_ERR(out_q15);
        return detail::q15ToFloat(out_q15.value());
    }

    std::expected<void, ErrorCode> stop()
    {
        return hw_utils_convertHalStatus(HAL_FMAC_FilterStop(&m_handle));
    }

  private:
    FMAC_HandleTypeDef                    &m_handle;
    const std::array<int16_t, NUM_B_TAPS> &m_coefB;
    const std::array<int16_t, NUM_A_TAPS> &m_coefA;
    const uint8_t                          m_gainExponent;
    int16_t                                m_outputSample = 0;
    uint16_t                               m_outputSize   = 1;

    std::expected<void, ErrorCode> pushSample(const int16_t sample_q15)
    {
        constexpr uint16_t size = 1;
        return hw_utils_convertHalStatus(HAL_FMAC_AppendFilterData(&m_handle, &sample_q15, &size));
    }

    std::expected<int16_t, ErrorCode> popSample()
    {
        RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_FMAC_PollFilterData(&m_handle, detail::kPollFilterDataTimeoutMs)));
        return m_outputSample;
    }
};

} // namespace hw::fmac
