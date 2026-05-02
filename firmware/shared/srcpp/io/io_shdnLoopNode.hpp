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
    const hw::gpio &pin;

  public:
    explicit node(const hw::gpio &in_pin_in, void (*in_can_broadcast)(bool))
      : can_broadcast(in_can_broadcast), pin(in_pin_in)
    {
    }
#elif TARGET_TEST
  private:
    bool status;

  public:
    explicit node(const bool in_status, void (*in_can_broadcast)(bool))
      : can_broadcast(in_can_broadcast), status(in_status)
    {
    }
    void set_status(bool in_status);
#endif

  public:
    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};

} // namespace io::shdn
