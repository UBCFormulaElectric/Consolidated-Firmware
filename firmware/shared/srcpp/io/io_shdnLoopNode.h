#pragma once
#include <functional>
#include "hw_gpio.h"

namespace io::shdn
{
class node
{
    const hw::gpio                  &pin;
    const std::function<void(bool)> &can_broadcast;

  public:
    explicit node(const hw::gpio &in_pin_in, const std::function<void(bool)> &in_can_broadcast)
      : pin(in_pin_in), can_broadcast(in_can_broadcast)
    {
    }

    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};
} // namespace io::shdn
