#pragma once
#include "hw_gpio.h"

namespace io::shdn
{
class node
{
    const hw::gpio &pin;
    void (*const can_broadcast)(bool);

  public:
    explicit node(const hw::gpio &in_pin_in, void (*in_can_broadcast)(bool))
      : pin(in_pin_in), can_broadcast(in_can_broadcast)
    {
    }

    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};
} // namespace io::shdn
