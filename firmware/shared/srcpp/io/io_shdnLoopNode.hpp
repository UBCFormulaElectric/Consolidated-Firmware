#pragma once
#ifdef TARGET_EMBEDDED
#include "hw_gpio.hpp"
#endif

namespace io::shdn
{

class node
{
#ifdef TARGET_EMBEDDED
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

  public:
    explicit node(bool in_status, void (*in_can_broadcast)(bool)) : status(in_status), can_broadcast(in_can_broadcast)
    {
    }
    void set_status(bool in_status);
#endif

  public:
    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};

} // namespace io::shdn
