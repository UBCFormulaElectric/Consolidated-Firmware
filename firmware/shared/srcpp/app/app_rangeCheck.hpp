#pragma once
#include <cstdint>
#include "util_utils.hpp"
namespace app
{
class RangeCheck
{
  public:
    enum class Status : uint8_t
    {
        VALUE_IN_RANGE,
        VALUE_UNDERFLOW,
        VALUE_OVERFLOW,
    };

  private:
    float min_value;
    float max_value;

  public:
    /* @brief Construct a range check object
     * @param min_value The minimum value of the range (exclusive)
     * @param max_value The maximum value of the range (exclusive)
     */
    explicit constexpr RangeCheck(float entry_min_value, float entry_max_value)
      : min_value(entry_min_value), max_value(entry_max_value)
    {
    }

    /* @brief Get the status of the given value with respect to the range check
     * @param value_to_check The value to check the status of
     * @return VALUE_IN_RANGE if the value is within the specified range
     *         VALUE_UNDERFLOW if the value is below the specified range
     *         VALUE_OVERFLOW if the value is above the specified range
     */
    [[nodiscard]] constexpr Status get_status(float value_to_check) const
    {
        if (value_to_check < min_value)
            return Status::VALUE_UNDERFLOW;
        else if (value_to_check > max_value)
            return Status::VALUE_OVERFLOW;
        else
            return Status::VALUE_IN_RANGE;
    }

    /* @brief Get the clamped value of the given value with respect to the range check
     * @param value_to_check The value to clamp
     * @return The clamped value of the given value with respect to the range check
     */
    [[nodiscard]] constexpr float get_clamped_value(float value_to_check) const
    {
        return CLAMP(value_to_check, min_value, max_value);
    }
};
} // namespace app