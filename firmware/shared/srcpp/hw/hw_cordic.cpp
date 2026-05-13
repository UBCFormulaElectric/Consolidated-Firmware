#include "hw_cordic.hpp"

#include <cstddef>

#include "hw_hal.hpp"

#include "main.h"
#ifndef HAL_CORDIC_MODULE_ENABLED
#error "HAL_CORDIC_MODULE_ENABLED must be defined and set to 1"
#endif

#include "hw_utils.hpp"

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
static constexpr uint32_t CORDIC_TIMEOUT = 10;

static std::expected<uint32_t, ErrorCode> nbWriteFromInputSize(const std::size_t input_size)
{
    switch (input_size)
    {
        case 1:
            return CORDIC_NBWRITE_1;
        case 2:
            return CORDIC_NBWRITE_2;
        default:
            return std::unexpected(ErrorCode::INVALID_ARGS);
    }
}

std::expected<int32_t, ErrorCode> calculate(const uint32_t func, const uint32_t scale, std::span<const int32_t> args)
{
    const auto nbwrite = nbWriteFromInputSize(args.size());
    if (not nbwrite)
    {
        return std::unexpected(nbwrite.error());
    }

    CORDIC_ConfigTypeDef config;

    config.Function  = func;
    config.Scale     = scale;
    config.InSize    = CORDIC_INSIZE_32BITS;
    config.OutSize   = CORDIC_OUTSIZE_32BITS;
    config.NbRead    = CORDIC_NBREAD_1;
    config.NbWrite   = nbwrite.value();
    config.Precision = CORDIC_PRECISION_15CYCLES;

    RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_CORDIC_Configure(&hcordic, &config)));

    int32_t result = 0;
    RETURN_IF_ERR(
        hw::utils::convertHalStatus(HAL_CORDIC_CalculateZO(&hcordic, args.data(), &result, 1, CORDIC_TIMEOUT)));
    return result;
}
}; // namespace hw::cordic
