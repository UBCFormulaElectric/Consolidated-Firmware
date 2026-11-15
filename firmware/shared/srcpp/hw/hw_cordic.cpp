#include "hw_cordic.hpp"

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
result<void> init()
{
    // Function, scale and number of arguments are set again by calculate() these are only defaults
    CORDIC_ConfigTypeDef config;

    config.Function  = CORDIC_FUNCTION_SINE;
    config.Scale     = CORDIC_SCALE_0;
    config.InSize    = CORDIC_INSIZE_32BITS;
    config.OutSize   = CORDIC_OUTSIZE_32BITS;
    config.NbRead    = CORDIC_NBREAD_1;
    config.NbWrite   = CORDIC_NBWRITE_1;
    config.Precision = PRECISION;

    // The HAL wrote CSR so make the next calculate() write it again
    current_csr = 0U;

    return hw::utils::convertHalStatus(HAL_CORDIC_Configure(&hcordic, &config));
}
}; // namespace hw::cordic
