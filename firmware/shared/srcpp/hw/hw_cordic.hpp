#pragma once

#ifdef STM32H733xx
#include "stm32h7xx_hal_cordic.h"
#else
#error "H5 version is not implemented yet. Currently can only be used by H7"
#endif
#include "util_errorCodes.hpp"

/**
 * STM32 CORDIC co-processor driver
 * Datasheet:
 * https://www.st.com/content/ccc/resource/training/technical/product_training/group1/67/7c/98/4f/37/38/4a/7f/STM32H7-Peripheral-Cordic_Coprocessor_CORDIC/files/STM32H7-Peripheral-Cordic_Coprocessor_CORDIC.pdf/_jcr_content/translations/en.STM32H7-Peripheral-Cordic_Coprocessor_CORDIC.pdf
 *
 * Hardware accelerates math operations such as:
 * Sine, Cosine, Phase, Modulus, Arctan,
 * Hyperbolic Sine, Hyperbolic Cosine, Hyperbolic Arctan
 * Natural log, Square root
 *
 * Note: Uses Fixed Point arithmetic internally
 */

// TODO: angles must be between [-1, 1], thus multiply by 1/pi then inverse to convert back

namespace hw
{
class Cordic
{
  private:
    const CORDIC_HandleTypeDef &handle;
    CORDIC_ConfigTypeDef        config;

  public:
    constexpr explicit Cordic(const CORDIC_HandleTypeDef &handle_in) : handle(handle_in) {}

    inline float sin(float angle, float hyp = 1.0f);
    inline float cos(float angle, float hyp = 1.0f);

    /**
     * Essentially a cartesian to polar conversion
     */
    inline float phase(float x, float y);
    /**
     * Calculates magnitude of a vector given its components
     */
    inline float mod(float x, float y);
    inline float atan(float x);
    inline float sinh(float angle);
    inline float cosh(float angle);
    inline float atanh(float angle);
    inline float ln(float x);
    inline float sqrt(float x);
}
} // namespace hw