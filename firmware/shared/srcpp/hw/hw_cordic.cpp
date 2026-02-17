#include "hw_cordic.hpp"

namespace hw::cordic
{

/**
 * @brief Configuret the CORDIC peripheral to:
 * - Perform the desired math operation
 * - Scaling factor
 * - Width of input and output data
 * - Number of 32 bit R/W expected before and after calculation
 * - Precision: 1 - 15 cycles (Higher -> More Precision)
 */
static inline ErrorCode configure()
{
    // CORDIC_ConfigTypeDef config;
    // config.Function = ;
    // config.Scale = ;
    // config.InSize = ;
    // config.OutSize = ;
    // config.NbRead = ;
    // config.Precision = ;
}
}; // namespace hw::cordic
