#pragma once
#include "app_math.hpp"

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cmath>

namespace app
{
namespace therm
{
    class ThermistorLUT
    {
      public:
        // Construct from explicit pointer + size
        constexpr ThermistorLUT(
            const float *resistances,
            float        starting_temp,
            float        resolution,
            std::size_t  size) noexcept
          : resistances_(resistances),
            starting_temp_(starting_temp),
            resolution_(resolution),
            size_((size <= 0xFFFFU) ? (uint16_t)size : 0U),
            valid_((size <= 0xFFFFU) && isNonIncreasing(resistances, size))
        {
            // Runtime check because `size` is not a compile-time constant here
            assert(size <= 0xFFFF && "LUT too large for uint16_t size");

            assert(valid_ && "Thermistor LUT must be non-increasing");
        }

        template <std::size_t N>
        constexpr explicit ThermistorLUT(const float (&arr)[N], float starting_temp, float resolution) noexcept
          : resistances_(arr),
            starting_temp_(starting_temp),
            resolution_(resolution),
            size_((uint16_t)N),
            valid_(isNonIncreasing(arr, N))
        {
            static_assert(N <= 0xFFFF, "LUT too large for uint16_t size");

            assert(valid_ && "Thermistor LUT must be non-increasing");
        }

        /**
         * Calculate temperature based on thermistor temperature and LUT
         * @param thermistor_resistance resistance of the thermistor
         * @param temp_resistance_lut reverse lookup table
         * @return Thermistor temperature in degrees C or -1 if out of bounds or invalid LUT
         */
        float resistanceToTemp(float thermistor_resistance) const noexcept
        {
            if (!valid_ || resistances_ == nullptr || size_ == 0U)
                return -1.0f;

            // Guard against NaN/inf inputs
            if (!std::isfinite(thermistor_resistance))
                return -1.0f;

            // Handle trivial single-entry LUT safely
            if (size_ == 1U)
            {
                return APPROX_EQUAL_FLOAT(thermistor_resistance, resistances_[0], 0.0001f) ? starting_temp_ : -1.0f;
            }

            // Ensure resistance is within bounds: resistances[0] is highest, resistances[size-1] is lowest
            if (!(thermistor_resistance <= resistances_[0] && thermistor_resistance >= resistances_[size_ - 1U]))
            {
                return -1.0f;
            }

            // Binary search for insertion point
            uint16_t low_index  = 0U;
            uint16_t high_index = static_cast<uint16_t>(size_ - 1);

            // Ensure low and high are a range for interpolation
            while (high_index > low_index + 1U)
            {
                uint16_t mid_index = low_index + (uint16_t)((high_index - low_index) / 2U);
                if (thermistor_resistance > resistances_[mid_index])
                {
                    // search lower half
                    if (mid_index == 0U)
                    {
                        high_index = 0U;
                        break;
                    }
                    high_index = (uint16_t)(mid_index);
                }
                else
                {
                    // search upper half
                    low_index = (uint16_t)(mid_index);
                }
            }

            const uint16_t therm_lut_index = low_index;

            // Linear interpolation between nearby LUT entries
            // We know that the index therm_lut_index is in bounds since we check
            // the lut size > 1, but also because low_index must be at most high_index - 1
            const float y2 = resistances_[therm_lut_index];
            const float y1 = resistances_[therm_lut_index + 1U];
            const float x2 = starting_temp_ + (float)therm_lut_index * resolution_;
            const float x1 = starting_temp_ + (float)(therm_lut_index + 1U) * resolution_;

            // guard against degenerate LUT entries
            if (APPROX_EQUAL_FLOAT(y2 - y1, 0.0f, 0.0001f))
                return x1;

            return (thermistor_resistance - y1) * ((x2 - x1) / (y2 - y1)) + x1;
        }

        // Accessors
        constexpr float        starting_temp() const noexcept { return starting_temp_; }
        constexpr float        resolution() const noexcept { return resolution_; }
        constexpr const float *resistances() const noexcept { return resistances_; }
        constexpr std::size_t  size() const noexcept { return size_; }
        constexpr bool         valid() const noexcept { return valid_; }

      private:
        static constexpr bool isNonIncreasing(const float *resistances, std::size_t size) noexcept
        {
            if (resistances == nullptr || size <= 1U)
                return true;

            for (std::size_t i = 1U; i < size; ++i)
            {
                if (!(resistances[i - 1U] >= resistances[i]))
                    return false;
            }

            return true;
        }

        const float   *resistances_;
        const float    starting_temp_;
        const float    resolution_;
        const uint16_t size_;
        const bool     valid_;
    };
} // namespace therm
} // namespace app