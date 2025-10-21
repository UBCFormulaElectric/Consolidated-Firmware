#pragma once
#ifdef TARGET_EMBEDDED
extern "C"
{
#include "hw/hal.hpp"
}
#endif

namespace hw
{
class Gpio
{
#ifdef TARGET_EMBEDDED
  private:
    GPIO_TypeDef *const port;
    const uint16_t      pin;

  public:
    explicit Gpio(GPIO_TypeDef *const port_in, const uint16_t pin_in) : port(port_in), pin(pin_in) {}
#endif
  public:
    [[nodiscard]] bool readPin() const;
    void               writePin(bool value) const;
};
} // namespace hw