#include <cmath>
#include <array>

#ifdef TARGET_EMBEDDED
#include "hw_cordic.hpp"
#endif
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

#ifdef TARGET_EMBEDDED
namespace
{
using namespace hw::cordic;

inline constexpr float CONVERSION     = static_cast<float>(1U << 31);
inline constexpr float CONVERSION_INV = 1.0f / CONVERSION;
inline constexpr float M_PI_F_INV     = 1.0f / M_PI_F;

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

inline int32_t convertToFixedPoint(float x)
{
    return static_cast<int32_t>(x * CONVERSION);
}

inline float calcScale(float x, uint32_t &scale)
{
    float scaled_x = x;
    scale          = 0U;

    while (std::abs(scaled_x) >= 1.0f && scale < 7U)
    {
        scaled_x *= 0.5f;
        scale++;
    }

    return scaled_x;
}

inline uint32_t cordicScale(uint32_t scale)
{
    switch (scale)
    {
        case 0:
            return CORDIC_SCALE_0;
        case 1:
            return CORDIC_SCALE_1;
        case 2:
            return CORDIC_SCALE_2;
        case 3:
            return CORDIC_SCALE_3;
        case 4:
            return CORDIC_SCALE_4;
        case 5:
            return CORDIC_SCALE_5;
        case 6:
            return CORDIC_SCALE_6;
        case 7:
            return CORDIC_SCALE_7;
        default:
            return CORDIC_SCALE_7;
    }
}
} // namespace
#endif

namespace io::math
{
// TODO: is it annoying for usability to have std::expected here? default to standard library implementation or arm math
// implementation?
// TODO: Extend to support vector operations
std::expected<float, ErrorCode> csin(float angle)
{
#ifdef TARGET_EMBEDDED
    std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
    std::array<int32_t, 1>       result{};

    const auto configure_result = configure(CORDIC_FUNCTION_SINE, CORDIC_SCALE_0, CORDIC_NBWRITE_1);
    if (not configure_result.has_value())
        return std::sin(angle);

    const auto calculate_result = calculate(1U, args, result);
    if (not calculate_result.has_value())
        return std::sin(angle);

    return convertToFloat(result[0]);
#else
    return std::sin(angle);
#endif
}

std::expected<float, ErrorCode> ccos(float angle)
{
#ifdef TARGET_EMBEDDED
    std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
    std::array<int32_t, 1>       result{};

    const auto configure_result = configure(CORDIC_FUNCTION_COSINE, CORDIC_SCALE_0, CORDIC_NBWRITE_1);
    if (not configure_result.has_value())
        return std::cos(angle);

    const auto calculate_result = calculate(1U, args, result);
    if (not calculate_result.has_value())
        return std::cos(angle);

    return convertToFloat(result[0]);
#else
    return std::cos(angle);
#endif
}

std::expected<float, ErrorCode> atan(float x)
{
#ifdef TARGET_EMBEDDED
    if (not std::isfinite(x))
        return std::unexpected(ErrorCode::OUT_OF_RANGE);

    uint32_t scale    = 0U;
    float    scaled_x = calcScale(x, scale);

    if (not std::isfinite(scaled_x) || std::abs(scaled_x) >= 1.0f)
        return std::atan(x);

    std::array<const int32_t, 1> args{ convertToFixedPoint(scaled_x) };
    std::array<int32_t, 1>       result{};

    const auto configure_result = configure(CORDIC_FUNCTION_ARCTANGENT, cordicScale(scale), CORDIC_NBWRITE_1);
    if (not configure_result.has_value())
        return std::atan(x);

    const auto calculate_result = calculate(1U, args, result);
    if (not calculate_result.has_value())
        return std::atan(x);

    float y = convertToFloat(result[0]);

    return y * static_cast<float>(1U << scale) * M_PI_F;
#else
    return std::atan(x);
#endif
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
