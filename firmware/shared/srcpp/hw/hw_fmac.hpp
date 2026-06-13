#pragma once

/**
 * @file hw_fmac.hpp
 * @brief C++ driver for the STM32 FMAC peripheral (IIR filter, polling mode).
 *
 * Reference documentation used for this driver:
 * - RM0481 (STM32H5 Reference manual on zotero): FMAC chapter — IIR formula, buffer layout (X1, X2, Y),
 *   sizes (P+ε, P+Q, Q+ε), q1.15 format, gain R, 256-word internal RAM.
 * - STM32H7 Filter Math Accelerator (FMAC) peripheral doc:
 *   https://www.st.com/content/ccc/resource/training/technical/product_training/group1/b3/5b/b1/ac/e1/61/40/b7/STM32H7-Peripheral-Filter_Math_Accelerator_FMAC/files/STM32H7-Peripheral-Filter_Math_Accelerator_FMAC.pdf/_jcr/content/translations/en.STM32H7-Peripheral-Filter_Math_Accelerator_FMAC.pdf
 *   Same concepts; H5 HAL follows the same model.
 * - stm32h5xx_hal_fmac.h (STM32CubeH5): HAL API — HAL_FMAC_FilterConfig, HAL_FMAC_FilterPreload,
 *   HAL_FMAC_FilterStart, HAL_FMAC_AppendFilterData, HAL_FMAC_PollFilterData, HAL_FMAC_FilterStop.
 */

extern "C"
{
#include "hw_hal.hpp"
#ifndef HAL_FMAC_MODULE_ENABLED
#error "HAL_FMAC_MODULE_ENABLED must be defined when building targets that use hw_fmac"
#endif
} /* gracefully handle the case where the module is not enabled, but fail to compile if it is not defined (prevents
 future linker errors if silently ignoring the error) */

#include "util_errorCodes.hpp"
#include <array>
#include <span>

namespace hw::fmac
{

// IIR coefficient limits from the FMAC reference manual (RM0481): P (B taps) and Q (A taps).
inline constexpr uint8_t kIirMaxBTaps = 64;
inline constexpr uint8_t kIirMaxATaps = 63;

class FmacIir
{
  public:
    explicit FmacIir(FMAC_HandleTypeDef &handle) : m_handle(handle) {}

    result<void> init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent);
    /** Process one raw int16 sample (e.g. IMU register count) through the IIR filter. */
    result<int16_t> processSample(int16_t sample_q15);
    /** Process one physical sample; maps ±full_scale to the FMAC's Q1.15 ±1.0 range and back. */
    result<float> process(float sample, float full_scale);
    result<void>  stop();

#ifdef HW_FMAC_DEBUG
    // TEMPORARY (remove with the rest of the HW_FMAC_DEBUG instrumentation).
    struct DebugStatus
    {
        uint32_t errorCode; // HAL_FMAC_ERROR_* bitfield (m_handle.ErrorCode)
        uint32_t statusReg; // FMAC_SR: X1FULL / YEMPTY / OVFL / UNFL / SAT live flags
    };
    [[nodiscard]] DebugStatus debugStatus() const;
#endif

  private:
    FMAC_HandleTypeDef &m_handle;
    // The HAL retains the pointers handed to HAL_FMAC_AppendFilterData / HAL_FMAC_FilterStart and only
    // dereferences them later during HAL_FMAC_PollFilterData, so the sample/size storage must outlive the
    // append->poll sequence. Keep them as members rather than locals to avoid dangling pointers.
    int16_t  m_inputSample  = 0;
    uint16_t m_inputSize    = 1;
    int16_t  m_outputSample = 0;
    uint16_t m_outputSize   = 1;

    // Stored filter configuration, kept so the filter can be re-armed if the polling handshake desyncs
    // at runtime (e.g. the START bit gets cleared under load). See armFilter().
    std::array<int16_t, kIirMaxBTaps> m_coefB{};
    std::array<int16_t, kIirMaxATaps> m_coefA{};
    uint8_t                           m_numBTaps     = 0;
    uint8_t                           m_numATaps     = 0;
    uint8_t                           m_gainExponent = 0;

    /** Stop, (re)configure, preload zero history and start the filter from the stored config. */
    result<void>    armFilter();
    result<void>    pushSample(int16_t sample_q15);
    result<int16_t> popSample();
};

} // namespace hw::fmac
