#pragma once
#ifdef TARGET_EMBEDDED
#include "hw_gpio.hpp"
#endif

namespace io::shdn
{
class node
{
    void (*const can_broadcast)(bool);
#ifdef TARGET_EMBEDDED
  private:
    const hw::Gpio &pin;

  public:
    explicit node(const hw::Gpio &in_pin_in, void (*in_can_broadcast)(bool))
      : pin(in_pin_in), can_broadcast(in_can_broadcast)
    {
    }
#elif TARGET_TEST
  public:
    explicit node(void (*in_can_broadcast)(bool)) : can_broadcast(in_can_broadcast) {}
#endif

    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};
} // namespace io::shdn
