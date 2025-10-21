#pragma once
#include "hw/gpio.hpp"

namespace io
{
class Switch
{
  public:
    const hw::Gpio &pin;
    explicit Switch(const hw::Gpio &pin_in) : pin(pin_in) {}

  public:
    [[nodiscard]] bool isClosed() const;
};
} // namespace io