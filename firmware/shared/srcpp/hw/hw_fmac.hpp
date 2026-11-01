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
 *   HAL_FMAC_FilterStart, HAL_FMAC_FilterStop.
 *
 * The HAL is only used for setup. process() talks to WDATA/SR/RDATA directly: HAL_FMAC_PollFilterData drops the
 * registered output buffer after the first read (so every later call returned a stale sample) and can return before
 * the output has been computed.
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
#include <span>

namespace hw::fmac
{

class FmacIir
{
  public:
    explicit FmacIir(FMAC_HandleTypeDef &handle) : m_handle(handle) {}

    /**
     * Configure and start the filter with zeroed history. Safe to call again to restart (e.g. after a TIMEOUT).
     * The FMAC computes y[n] = 2^R * (sum_k b_q[k] x[n-k] + sum_j a_q[j] y[n-j]), so for a standard design
     * (b, a) with a[0] = 1 pass, in Q1.15:
     * @param coefB b[0..P-1] / 2^R
     * @param coefA -a[1..Q] / 2^R (note the sign: the FMAC adds the feedback terms)
     * @param gainExponent R, the smallest value that makes every scaled coefficient fit in [-1, 1)
     */
    result<void> init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent);
    /**
     * Process one sample: float in → FMAC (Q1.15) → float out in the success value.
     * Inputs are rounded and saturated to [-1, 1). Returns ERROR if init() has not started the filter, and TIMEOUT
     * if the FMAC produced no output; call init() again before continuing.
     */
    result<float> process(float sample);
    result<void>  stop();

  private:
    FMAC_HandleTypeDef &m_handle;
};

} // namespace hw::fmac
