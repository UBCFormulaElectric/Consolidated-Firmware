#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_gpio.hpp"
#endif
namespace io
{
class Switch
{
#ifdef TARGET_EMBEDDED
  public:
    const hw::Gpio &pin;

    explicit Switch(const hw::Gpio &pin_in) : pin(pin_in) {}
#endif

  public:
    [[nodiscard]] bool isClosed() const;
};
} // namespace io
