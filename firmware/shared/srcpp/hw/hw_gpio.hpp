#pragma once
#ifdef TARGET_EMBEDDED
extern "C"
{
#include "hw_hal.hpp"
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
    // note that this cannot be constexpr as GPIO_TypeDef is provided as a literal type by stupid ahh HAL
    explicit Gpio(GPIO_TypeDef *const port_in, const uint16_t pin_in) : port(port_in), pin(pin_in) {}
    [[nodiscard]] uint16_t getPin() const { return pin; }
#endif
  public:
    [[nodiscard]] bool readPin() const
#ifdef TARGET_EMBEDDED
    {
        return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
    }
#else
        ;
#endif

    void writePin(const bool value) const
#ifdef TARGET_EMBEDDED
    {
        HAL_GPIO_WritePin(port, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
#else
        ;
#endif
};
} // namespace hw