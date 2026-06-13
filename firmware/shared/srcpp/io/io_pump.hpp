#pragma once

#include "util_errorCodes.hpp"
#include "io_pump.hpp"
#include "io_potentiometer.hpp"

#include <expected>

namespace io
{
class pump
{
  public:
#ifdef TARGET_EMBEDDED
    constexpr explicit pump(const Potentiometer &pot, const bool invert) : pot_{ pot }, invert_{ invert } {}
#elif defined(TARGET_TEST)
    constexpr explicit pump() {}
#endif

    result<void>    setPercentage(uint8_t percentage) const;
    result<uint8_t> getPercentage() const;

  private:
    static constexpr uint8_t logicalToHw(const bool invert, const uint8_t percent)
    {
        return invert ? static_cast<uint8_t>(100u - percent) : percent;
    }

    static constexpr uint8_t hwToLogical(const bool invert, const uint8_t percent)
    {
        return invert ? static_cast<uint8_t>(100u - percent) : percent;
    }

#ifdef TARGET_EMBEDDED
    const Potentiometer &pot_;
    bool                 invert_;
#endif
};
} // namespace io