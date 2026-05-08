#include "hw_cordic.hpp"

#ifdef STM32H562xx
#include "stm32h5xx_hal_cordic.h"
#elif STM32H733xx
#include "stm32h7xx_hal_cordic.h"
#endif

#include "hw_hal.hpp"

#include "main.h"
#ifndef HAL_CORDIC_MODULE_ENABLED
#error "HAL_CORDIC_MODULE_ENABLED must be defined and set to 1"
#endif

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
static CORDIC_ConfigTypeDef config;
static constexpr uint32_t   CORDIC_TIMEOUT = 10;

/**
 * @brief Configure the CORDIC peripheral to:
 * - Perform the desired math operation
 * - Scaling factor
 * - Width of input and output data
 * - Number of 32 bit R/W expected before and after calculation
 * - Precision: 1 - 15 cycles (Higher -> More Precision)
 */
std::expected<void, ErrorCode> configure(uint32_t func, uint32_t scale, uint32_t nbwrite)
{
    config.Function  = func;
    config.Scale     = scale;
    config.InSize    = CORDIC_INSIZE_32BITS;
    config.OutSize   = CORDIC_OUTSIZE_32BITS;
    config.NbRead    = CORDIC_NBREAD_1;
    config.NbWrite   = nbwrite;
    config.Precision = CORDIC_PRECISION_15CYCLES;

    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_CORDIC_Configure(&hcordic, &config)));
    return {};
}

std::expected<void, ErrorCode> calculate(uint32_t nbwrite, std::span<const int32_t> args, std::span<int32_t> result)
{
    RETURN_IF_ERR(hw::utils::convertHalStatus(
        HAL_CORDIC_CalculateZO(&hcordic, args.data(), result.data(), nbwrite, CORDIC_TIMEOUT)));
    return {};
}
}; // namespace hw::cordic
