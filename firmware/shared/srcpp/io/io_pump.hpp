#pragma once

#include <cstdint>
#include <expected>
#include "util_errorCodes.hpp"
#include "io_efuses.hpp"
#include "io_pump.hpp"
#include "io_potentiometer.hpp"

namespace io
{
class pump
{
  public:
#ifdef TARGET_EMBEDDED
    constexpr explicit pump(const Potentiometer &pot, bool invert, const Efuse &efuse)
      : pot_{ pot }, invert_{ invert }, efuse_{ efuse }
    {
    }
#elif defined(TARGET_TEST)
    constexpr explicit pump() {}
#endif

    result<void>    setPercentage(uint8_t percentage) const;
    result<uint8_t> getPercentage() const;
    result<void>    enable(bool enable) const;
    result<bool>    isEnabled() const;
    result<bool>    ok() const;
    result<bool>    isReady() const;

  private:
    static constexpr uint8_t logicalToHw(bool invert, uint8_t percent)
    {
        return invert ? static_cast<uint8_t>(100u - percent) : percent;
    }

    static constexpr uint8_t hwToLogical(bool invert, uint8_t percent)
    {
        return invert ? static_cast<uint8_t>(100u - percent) : percent;
    }

#ifdef TARGET_EMBEDDED
    const Potentiometer &pot_;
    bool                 invert_;
    const Efuse         &efuse_;
#endif
};
} // namespace io