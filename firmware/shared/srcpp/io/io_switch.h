#pragma once
#include "hw_gpio.h"

namespace io
{
class Switch
{
  public:
    const hw::gpio &pin;
    explicit Switch(const hw::gpio &pin_in) : pin(pin_in) {}

  public:
    [[nodiscard]] bool isClosed() const;
};
} // namespace io