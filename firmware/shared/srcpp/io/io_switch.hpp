#pragma once

#include <cstdint>
#include "hw_gpio.hpp"

namespace io
{
class Switch
{
  private:
    /**
     * Note:
     *
     * Debounce duration is dependant on task cadence.
     * Thus if this switch is polled in a 100Hz task,
     * the debounce duration in ms would be:
     *
     * Actual Duration = debounce_ticks * 10ms
     */
    const uint16_t debounce_ticks;
    uint16_t       elapsed        = 0;
    bool           last_state_raw = false;
    bool           state          = false;

  public:
    const hw::Gpio &pin;

    explicit Switch(const uint16_t debounce_ticks_in, const hw::Gpio &pin_in)
      : debounce_ticks(debounce_ticks_in), pin(pin_in)
    {
        last_state_raw = pin.readPin();
        state          = last_state_raw;
    }
    [[nodiscard]] bool isClosed();
};
} // namespace io