#pragma once

#include <cstdint>
#include <span>

#include "hw_hal.hpp"
#include "util_errorCodes.hpp"

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
// 6 cycles (24 iterations) is the maximum useful precision for q1.31 results (AN5325), more only costs time.
inline constexpr uint32_t PRECISION = CORDIC_PRECISION_6CYCLES;
// CSR fields that are the same for every calculation: q1.31 (32-bit) in and out, one result.
inline constexpr uint32_t CSR_FIXED_FIELDS = PRECISION | CORDIC_INSIZE_32BITS | CORDIC_OUTSIZE_32BITS | CORDIC_NBREAD_1;

namespace
{
    // CSR value last written by calculate(), only rewritten when the function changes. 0 is never a value
    // This is necessary to support multiple operations and not just be restricted to sin or cosine for example.
    inline uint32_t current_csr = 0U;
} // namespace

/**
 * Configures the CORDIC (precision, data sizes) through the HAL
 */
result<void> init();

/**
 * Runs one calculation with direct register access (CSR, WDATA, RDATA), no HAL calls. Inline so callers such as
 * io::math compile down to the register accesses. CSR is only written when the function, scale or number of
 * arguments changes.
 */
inline result<int32_t> calculate(const uint32_t func, const uint32_t scale, const std::span<const int32_t> args)
{
    if (args.empty() || args.size() > 2)
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    const uint32_t csr = func | scale | (args.size() == 2 ? CORDIC_NBWRITE_2 : CORDIC_NBWRITE_1) | CSR_FIXED_FIELDS;
    if (csr != current_csr)
    {
        WRITE_REG(CORDIC->CSR, csr);
        current_csr = csr;
    }

    // Zero-overhead mode: the calculation starts when the last argument is written, and reading RDATA
    // stalls the bus until the result is ready. Takes approx ~56 cycles from io::math to here, about
    // a 3x improvement from the standard library. We do not configure it to use Interrupt or DMA because
    // the cycle count in zero overhead mode is so low, where context switches may start becoming expensive
    for (const int32_t arg : args)
    {
        WRITE_REG(CORDIC->WDATA, static_cast<uint32_t>(arg));
    }
    return static_cast<int32_t>(READ_REG(CORDIC->RDATA));
}
}; // namespace hw::cordic
