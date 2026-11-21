#include "app_button.hpp"
#include "app_signal.hpp"
#include "io_ntpButton.hpp"

namespace app::button
{
namespace
{
    constexpr uint32_t DEBOUNCE_MS = 50;
}

bool ntpWasJustPressed()
{
    static app::Signal signal{ DEBOUNCE_MS, DEBOUNCE_MS };
    static bool        prev_active = false;

    const bool raw    = io::ntpButton::isPressed();
    const bool active = signal.get_updated_state(raw, !raw) == app::Signal::SignalState::ACTIVE;

    const bool rising = active && !prev_active;
    prev_active       = active;
    return rising;
}
} // namespace app::button
