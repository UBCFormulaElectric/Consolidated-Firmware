#include "hw_cordic.hpp"
#include "util_errorCodes.hpp"

namespace hw::cordic
{
inline constexpr uint32_t CORDIC_TIMEOUT = 10U;

/**
 * @brief Configuret the CORDIC peripheral to:
 * - Perform the desired math operation
 * - Scaling factor
 * - Width of input and output data
 * - Number of 32 bit R/W expected before and after calculation
 * - Precision: 1 - 15 cycles (Higher -> More Precision)
 */
static inline std::expected<void, ErrorCode> configure(
    CORDIC_Function          func,
    CORDIC_Scale             scale,
    CORDIC_Nb_Write          nbwrite,
    std::span<const int32_t> args,
    std::span<int32_t>       res)
{
    config.Function  = func;
    config.Scale     = scale;
    config.InSize    = CORDIC_INSIZE_32BITS;
    config.OutSize   = CORDIC_OUTSIZE_32BITS;
    config.NbRead    = CORDIC_NBREAD_1;
    config.NbWrite   = nbwrite;
    config.Precision = CORDIC_PRECISION_15CYCLES;

    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_CORDIC_Configure(handle, &config)));
    return {};
}
static inline std::expected<void, ErrorCode> configure(CORDIC_Nb_Write nbwrite, std::span<const int32_t> args)
{
    RETURN_IF_ERR(
        hw_utils_convertHalStatus(HAL_CORDIC_Calculate(handle, args.data(), res.data(), nbwrite, CORDIC_TIMEOUT)));
    return {};
}

inline float sin(float angle, float hyp = 1.0f)
{
    int32_t fp_angle = static_cast<int32_t>(angle * (1 << 31));
}
inline float cos(float angle, float hyp = 1.0f) {}
inline float phase(float x, float y) {}
inline float mod(float x, float y) {}
inline float atan(float x) {}
inline float sinh(float angle) {}
inline float cosh(float angle) {}
inline float atanh(float angle) {}
inline float ln(float x) {}
inline float sqrt(float x) {}
}; // namespace hw::cordic
