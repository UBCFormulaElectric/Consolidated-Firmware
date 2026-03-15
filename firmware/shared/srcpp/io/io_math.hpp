#include <array>

#include "hw_cordic.hpp"
#include "util_errorCodes.hpp"
#include "util_units.hpp"

/**
 * Faster math operations using CORDIC co-processor
 */

/**
 * Note:
 *
 * Cordic cosine and sine operations also take a modulus argument. However we would need to convert this modulus
 * to fixed point and scale it between -1 and 1 and then rescale the output which would take unecessary cycles.
 * Thus the we only pass in angle and the user can simply multiply the result by the desired modulus outside. of
 * the cordic driver.
 */

using namespace hw::cordic;

namespace io::math
{
// TODO: is it annoying for usability to have std::expected here? default to standard library implementation or arm math
// implementation?
// TODO: Extend to support vector operations
std::expected<float, ErrorCode> csin(float angle)
{
    std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
    std::array<int32_t, 1>       result{};

    RETURN_IF_ERR(configure(CORDIC_FUNCTION_SINE, CORDIC_SCALE_0, CORDIC_NBWRITE_1));
    RETURN_IF_ERR(calculate(1U, args, result));

    return convertToFloat(result[0]);
}
std::expected<float, ErrorCode> ccos(float angle)
{
    std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
    std::array<int32_t, 1>       result{};

    RETURN_IF_ERR(configure(CORDIC_FUNCTION_COSINE, CORDIC_SCALE_0, CORDIC_NBWRITE_1));
    RETURN_IF_ERR(calculate(1U, args, result));

    return convertToFloat(result[0]);
}

// inline float phase(float x, float y) {}
// inline float mod(float x, float y) {}
// inline float atan(float x) {}
// inline float sinh(float angle) {}
// inline float cosh(float angle) {}
// inline float atanh(float angle) {}
// inline float ln(float x) {}
// inline float sqrt(float x) {}
}; // namespace io::math