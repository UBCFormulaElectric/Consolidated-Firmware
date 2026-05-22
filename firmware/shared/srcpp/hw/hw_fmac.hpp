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
#include <span>

namespace hw::fmac
{

class FmacIir
{
  public:
    explicit FmacIir(FMAC_HandleTypeDef &handle) : m_handle(handle) {}

    result<void> init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent);
    /** Process one sample: float in → FMAC (Q1.15) → float out in the success value. */
    result<float> process(const float sample);
    result<void>  stop();

  private:
    FMAC_HandleTypeDef &m_handle;
    int16_t             m_outputSample = 0;
    uint16_t            m_outputSize   = 1;
    result<void>        pushSample(int16_t sample_q15);
    result<int16_t>     popSample();
};

} // namespace hw::fmac
