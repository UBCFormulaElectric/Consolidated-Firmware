#include <span>

#include "hw_hal.hpp"

#ifdef STM32H562xx
#include "stm32h5xx_hal_cordic.h"
#elif STM32H733xx
#include "stm32h7xx_hal_cordic.h"
#endif

#include "main.h"
#ifndef HAL_CORDIC_MODULE_ENABLED
#error "HAL_CORDIC_MODULE_ENABLED must be defined and set to 1"
#endif

#include "hw_utils.hpp"
#include "util_errorCodes.hpp"
#include "util_units.hpp"

/**
 * STM32 CORDIC co-processor driver
 * Datasheet:
 * https://www.st.com/content/ccc/resource/training/technical/product_training/group1/67/7c/98/4f/37/38/4a/7f/STM32H7-Peripheral-Cordic_Coprocessor_CORDIC/files/STM32H7-Peripheral-Cordic_Coprocessor_CORDIC.pdf/_jcr_content/translations/en.STM32H7-Peripheral-Cordic_Coprocessor_CORDIC.pdf
 *
 * Hardware accelerates math operations such as:
 * Sine
 * Cosine
 * Phase
 * Modulus
 * Arctan
 * Hyperbolic Sine
 * Hyperbolic Cosine
 * Hyperbolic Arctan
 * Natural log
 * Square root
 *
 * Note: Uses Fixed Point arithmetic internally
 */

// TODO: Add Cordic status check

namespace hw::cordic
{
inline constexpr float CONVERSION     = static_cast<float>(1U << 31);
inline constexpr float CONVERSION_INV = 1.0f / CONVERSION;

inline int32_t convertAngleToFixedPoint(float angle)
{
    constexpr float M_PI_INV = 1.0f / M_PI_F;

    // TODO: can we optimize fmodf?
    float mod = std::fmodf(angle, M_2PI_F);
    if (mod > M_PI_F)
    {
        mod -= M_2PI_F;
    }
    else if (mod < -M_PI_F)
    {
        mod += M_2PI_F;
    }

    return static_cast<int32_t>(mod * M_PI_INV * CONVERSION);
}

inline float convertToFloat(int32_t fixed_point)
{
    return static_cast<float>(fixed_point) * CONVERSION_INV;
}

/**
 * @brief Configure the CORDIC peripheral to:
 * - Perform the desired math operation
 * - Scaling factor
 * - Width of input and output data
 * - Number of 32 bit R/W expected before and after calculation
 * - Precision: 1 - 15 cycles (Higher -> More Precision)
 */
std::expected<void, ErrorCode> configure(uint32_t func, uint32_t scale, uint32_t nbwrite);

std::expected<void, ErrorCode>
    calculate(uint32_t nbwrite, std::span<const int32_t> args, std::span<int32_t> result);
}; // namespace hw::cordic
