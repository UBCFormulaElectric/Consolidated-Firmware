#pragma once

#include <cstdint>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.hpp"
#endif
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

#ifdef TARGET_EMBEDDED
    const hw::Gpio &pin;
#endif

  public:
#ifdef TARGET_EMBEDDED
    explicit Switch(const uint16_t debounce_ticks_in, const hw::Gpio &pin_in)
      : debounce_ticks(debounce_ticks_in), pin(pin_in)
    {
    }
#else
    explicit Switch(const uint16_t debounce_ticks_in) : debounce_ticks(debounce_ticks_in) {}
#endif

    void               init();
    [[nodiscard]] bool isClosed();

#ifdef TARGET_TEST
    void setState(const bool state);
#endif
};
} // namespace io