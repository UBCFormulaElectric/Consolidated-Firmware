#include "io_rotary.hpp"
#include "hw_gpios.hpp"

namespace io::rotary
{
// Callback function pointers.
static volatile RotaryCallback clockwise_callback        = nullptr;
static volatile RotaryCallback counterclockwise_callback = nullptr;
static volatile RotaryCallback push_callback             = nullptr;

// Last encoder state (two-bit value: A<<1 | B).
static uint8_t prev_state = 0;

static uint8_t get_current_state()
{
    return static_cast<uint8_t>(rot_a.readPin() << 1) | rot_b.readPin();
}

void init()
{
    // Read initial state of A and B.
    prev_state = get_current_state();
}
void setClockwiseCallback(const RotaryCallback cb)
{
    clockwise_callback = cb;
}
void setCounterClockwiseCallback(const RotaryCallback cb)
{
    counterclockwise_callback = cb;
}
void io_rotary_setPushCallback(const RotaryCallback cb)
{
    push_callback = cb;
}

/**
 * @brief  Sample A/B pins, compute direction via Gray-code state machine,
 *         and invoke the appropriate rotation callback.
 *         Called from the EXTI interrupt routine.
 */
static void rotary_update()
{
    // Read current state.
    const uint8_t state             = get_current_state();
    bool          turn_is_clockwise = false;

    /*
     * Rotary encoder state transitions:
     *
     *   Bit pattern:  A B
     *     State 0:    0 0
     *     State 1:    0 1
     *     State 2:    1 0
     *     State 3:    1 1
     *
     * A clockwise detent is detected when the encoder moves through these
     * transitions in order:
     *
     *     0 -> 2 -> 3 -> 1 -> 0
     *
     * In code we simply check each possible prev_state → state match:
     *   0 → 2,  2 → 3,  3 → 1,  1 → 0
     *
     * Any opposite transition (e.g. 0→1, 1→3, 3→2, 2→0) would be
     * counter-clockwise.
     */
    switch (prev_state)
    {
        case 0:
            if (state == 2)
                turn_is_clockwise = true;
            else if (state == 1)
                turn_is_clockwise = false;
            else
                LOG_ERROR("NONSENSE");
            break;
        case 1:
            if (state == 0)
                turn_is_clockwise = true;
            else if (state == 3)
                turn_is_clockwise = false;
            else
                LOG_ERROR("NONSENSE");
            break;
        case 2:
            if (state == 3)
                turn_is_clockwise = true;
            else if (state == 0)
                turn_is_clockwise = false;
            else
                LOG_ERROR("NONSENSE");
            break;
        case 3:
            if (state == 1)
                turn_is_clockwise = true;
            else if (state == 2)
                turn_is_clockwise = false;
            else
                LOG_ERROR("NONSENSE");
            break;
        default:
            break;
    }

    prev_state = state;

    if (turn_is_clockwise and clockwise_callback)
    {
        clockwise_callback();
    }
    else if (not turn_is_clockwise and counterclockwise_callback)
    {
        counterclockwise_callback();
    }
}
void rotA_rotB_IRQHandler()
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_a.getPin());
    __HAL_GPIO_EXTI_CLEAR_IT(rot_b.getPin());
    rotary_update();
}
void push_IRQHandler()
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_s.getPin());
    if (push_callback)
    {
        push_callback();
    }
}
} // namespace io::rotary