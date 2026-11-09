#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>

#ifdef TARGET_EMBEDDED
#include "hw_cordic.hpp"
#include "util_units.hpp"
#endif

#ifdef TARGET_EMBEDDED
namespace io::math
{
namespace
{
    inline constexpr float CONVERSION     = static_cast<float>(1U << 31);
    inline constexpr float CONVERSION_INV = 1.0f / CONVERSION;

    inline int32_t convertAngleToFixedPoint(const float angle)
    {
        constexpr float TURNS_PER_RAD = 1.0f / M_2PI_F;
        constexpr float Q31_PER_TURN  = 2.0f * CONVERSION; // q1.31 spans [-pi, pi), so one turn is 2^32

        // Wrap to [-0.5, 0.5] turns by subtracting the nearest whole turn.
        // nearbyint is a single vrintr on the M7 FPU,
        const float turns   = angle * TURNS_PER_RAD;
        const float wrapped = turns - std::nearbyint(turns);

        // +0.5 turn (+pi) does not fit in q1.31, but it is the same angle as -pi, which does.
        float fixed = wrapped * Q31_PER_TURN;
        if (fixed >= CONVERSION)
        {
            fixed -= Q31_PER_TURN;
        }
        return static_cast<int32_t>(fixed);
    }

    inline float convertToFloat(const int32_t fixed_point)
    {
        return static_cast<float>(fixed_point) * CONVERSION_INV;
    }

    inline int32_t convertToFixedPoint(const float x)
    {
        return static_cast<int32_t>(x * CONVERSION);
    }

    inline float calcScale(const float x, uint32_t &scale)
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

    inline uint32_t cordicScale(const uint32_t scale)
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

    inline float cordicSin(const float angle)
    {
        const std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
        const auto                         result = hw::cordic::calculate(CORDIC_FUNCTION_SINE, CORDIC_SCALE_0, args);
        return result.has_value() ? convertToFloat(result.value()) : std::sin(angle);
    }

    inline float cordicCos(const float angle)
    {
        const std::array<const int32_t, 1> args{ convertAngleToFixedPoint(angle) };
        const auto                         result = hw::cordic::calculate(CORDIC_FUNCTION_COSINE, CORDIC_SCALE_0, args);
        return result.has_value() ? convertToFloat(result.value()) : std::cos(angle);
    }

    inline float cordicAtan(const float x)
    {
        // The CORDIC takes |x| < 1, with input scaling up to 2^7. Anything outside that (or not finite) goes to std.
        uint32_t    scale    = 0U;
        const float scaled_x = calcScale(x, scale);
        if (not std::isfinite(scaled_x) || std::abs(scaled_x) >= 1.0f)
            return std::atan(x);

        const std::array<const int32_t, 1> args{ convertToFixedPoint(scaled_x) };
        const auto result = hw::cordic::calculate(CORDIC_FUNCTION_ARCTANGENT, cordicScale(scale), args);
        if (not result.has_value())
            return std::atan(x);

        return convertToFloat(result.value()) * static_cast<float>(1U << scale) * M_PI_F;
    }
} // namespace
} // namespace io::math
#endif

namespace io::math::fast
{
// Decimal types these functions take. They are templates (not plain float / double overloads) so an autodiff
// expression can never convert to a plain number by accident and lose its derivative: that is a compile error instead.
template <typename T>
concept Decimal = std::same_as<T, float> || std::same_as<T, double>;

template <Decimal T> [[nodiscard]] inline T sin(const T x)
{
#ifdef TARGET_EMBEDDED
    if constexpr (std::same_as<T, float>)
    {
        return cordicSin(x);
    }
    else
#endif
    {
        return std::sin(x);
    }
}

template <Decimal T> [[nodiscard]] inline T cos(const T x)
{
#ifdef TARGET_EMBEDDED
    if constexpr (std::same_as<T, float>)
    {
        return cordicCos(x);
    }
    else
#endif
    {
        return std::cos(x);
    }
}

template <Decimal T> [[nodiscard]] inline T atan(const T x)
{
#ifdef TARGET_EMBEDDED
    if constexpr (std::same_as<T, float>)
    {
        return cordicAtan(x);
    }
    else
#endif
    {
        return std::atan(x);
    }
}

template <Decimal T> [[nodiscard]] inline T sinatan(const T x)
{
    return x / std::sqrt(T(1.0) + (x * x));
}

template <Decimal T> [[nodiscard]] inline T cosatan(const T x)
{
    return T(1.0) / std::sqrt(T(1.0) + (x * x));
}

// inline float phase(float x, float y) {}
// inline float mod(float x, float y) {}
// inline float sinh(float angle) {}
// inline float cosh(float angle) {}
// inline float atanh(float angle) {}
// inline float ln(float x) {}
// inline float sqrt(float x) {}
} // namespace io::math::fast
