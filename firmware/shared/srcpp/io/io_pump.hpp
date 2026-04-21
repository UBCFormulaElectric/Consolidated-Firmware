#pragma once

#include <cstdint>
#include <expected>
#include "util_errorCodes.hpp"
#include "io_efuses.hpp"
#include "io_pump.hpp"
#include "io_potentiometer.hpp"

namespace io
{
class Pump
{
  public:
#ifdef TARGET_EMBEDDED
    constexpr explicit Pump(const Potentiometer &pot, bool invert, Efuse &efuse)
      : pot_{ pot }, invert_{ invert }, efuse_{ efuse }
    {
    }
#elif defined(TARGET_TEST)
    constexpr explicit Pump() {}
#endif

    std::expected<void, ErrorCode>    setPercentage(uint8_t percentage) const;
    std::expected<uint8_t, ErrorCode> getPercentage() const;
    std::expected<void, ErrorCode>    enable(bool enable) const;
    std::expected<bool, ErrorCode>    isEnabled() const;
    std::expected<bool, ErrorCode>    ok() const;
    std::expected<bool, ErrorCode>    isReady() const;

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
    Efuse               &efuse_;
#endif
};
} // namespace io