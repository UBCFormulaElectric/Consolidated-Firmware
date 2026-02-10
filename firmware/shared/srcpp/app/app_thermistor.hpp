#pragma once
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
	constexpr ThermistorLUT(float starting_temp, float resolution, const float *resistances, std::size_t size) noexcept
	    : starting_temp_(starting_temp), resolution_(resolution), resistances_(resistances), size_((uint16_t)size)
	{
		// Runtime check because `size` is not a compile-time constant here
		assert(size <= 0xFFFF && "LUT too large for uint16_t size");
	}

	template <std::size_t N>
	constexpr explicit ThermistorLUT(float starting_temp, float resolution, const float (&arr)[N]) noexcept
	    : starting_temp_(starting_temp), resolution_(resolution), resistances_(arr), size_((uint16_t)N)
	{
		static_assert(N <= 0xFFFF, "LUT too large for uint16_t size");
	}

    /**
     * Calculate temperature based on thermistor temperature and LUT
     * @param thermistor_resistance resistance of the thermistor
     * @param temp_resistance_lut reverse lookup table
     * @return Thermistor temperature in degrees C or -1 if out of bounds or invalid LUT
     */
	float resistanceToTemp(float thermistor_resistance) const noexcept
	{
		if (resistances_ == nullptr || size_ == 0U)
			return -1.0f;

		// Guard against NaN/inf inputs
		if (!std::isfinite(thermistor_resistance))
			return -1.0f;

		// Handle trivial single-entry LUT safely
		if (size_ == 1U)
		{
			return thermistor_resistance == resistances_[0] ? starting_temp_ : -1.0f;
		}

		// Ensure resistance is within bounds: resistances[0] is highest, resistances[size-1] is lowest
		if (!(thermistor_resistance <= resistances_[0] &&
		      thermistor_resistance >= resistances_[size_ - 1U]))
		{
			return -1.0f;
		}

		// Binary search for insertion point
		uint16_t low_index  = 0U;
		uint16_t high_index = size_ - 1U;

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
		if (y2 == y1)
			return x1;

		return (thermistor_resistance - y1) * ((x2 - x1) / (y2 - y1)) + x1;
	}

	// Accessors
	constexpr float starting_temp() const noexcept { return starting_temp_; }
	constexpr float resolution() const noexcept { return resolution_; }
	constexpr const float *resistances() const noexcept { return resistances_; }
	constexpr std::size_t size() const noexcept { return size_; }

private:
	const float 	*resistances_;
	const float		starting_temp_;
	const float     resolution_;
	const uint16_t  size_;
};
} // namespace therm
} // namespace app