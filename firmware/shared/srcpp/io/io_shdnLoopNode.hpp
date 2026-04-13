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
#endif
    bool status                 = false;
    void (*can_broadcast)(bool) = nullptr;

  public:
#ifdef TARGET_EMBEDDED
    explicit node(const hw::Gpio &in_pin_in, void (*in_can_broadcast)(bool))
      : pin(in_pin_in), can_broadcast(in_can_broadcast)
    {
    }
#elif TARGET_TEST
  private:
    bool status;

  public:
    explicit node(bool in_status, void (*in_can_broadcast)(bool)) : can_broadcast(in_can_broadcast), status(in_status)
    {
    }
    void set_status(bool in_status);
#endif

  public:
    [[nodiscard]] bool is_ok() const;
    void               broadcast() const;
};

} // namespace io::shdn
