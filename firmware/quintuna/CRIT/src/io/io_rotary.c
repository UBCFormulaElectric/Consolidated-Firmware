#include "io_rotary.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "io_log.h"
#include <stdint.h>

// Callback function pointers.
static volatile RotaryCallback clockwise_callback        = 0;
static volatile RotaryCallback counterclockwise_callback = 0;
static volatile RotaryCallback push_callback             = 0;

// Last encoder state (two-bit value: A<<1 | B).
static uint8_t prev_state = 0;

/**
 * @brief  Initialize the rotary encoder module.
 *         Reads the initial levels of A and B pins to set prev state.
 */
void io_rotary_init(void)
{
    // Read initial state of A and B.
    uint8_t a  = hw_gpio_readPin(&rot_a);
    uint8_t b  = hw_gpio_readPin(&rot_b);
    prev_state = (uint8_t)(a << 1) | b;
}

/**
 * @brief  Set a callback to be called on each cw event.
 * @param  cb  Function pointer called when encoder turns cw.
 */
void io_rotary_setClockwiseCallback(RotaryCallback cb)
{
    clockwise_callback = cb;
}

/**
 * @brief  Set a callback to be called on each ccw event.
 * @param  cb  Function pointer called when encoder turns ccw.
 */
void io_rotary_setCounterClockwiseCallback(RotaryCallback cb)
{
    counterclockwise_callback = cb;
}

/**
 * @brief  Set a callback to be called on each push event.
 * @param  cb  Function pointer called when encoder pushes.
 */
void io_rotary_setPushCallback(RotaryCallback cb)
{
    push_callback = cb;
}

/**
 * @brief  Sample A/B pins, compute direction via Gray-code state machine,
 *         and invoke the appropriate rotation callback.
 *         Called from the EXTI interrupt routine.
 */
static void rotary_update(void)
{
    // Read current state.
    uint8_t a              = (uint8_t)hw_gpio_readPin(&rot_a);
    uint8_t b              = (uint8_t)hw_gpio_readPin(&rot_b);
    uint8_t state          = (uint8_t)(a << 1) | b;
    bool    clockwise_turn = false;

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
            {
                clockwise_turn = true;
            }
            break;
        case 1:
            if (state == 0)
            {
                clockwise_turn = true;
            }
            break;
        case 2:
            if (state == 3)
            {
                clockwise_turn = true;
            }
            break;
        case 3:
            if (state == 1)
            {
                clockwise_turn = true;
            }
            break;
        default:
            break;
    }

    prev_state = state;

    if ((clockwise_turn == true) && (clockwise_callback))
    {
        clockwise_callback();
    }
    else if ((clockwise_turn == false) && (counterclockwise_callback))
    {
        counterclockwise_callback();
    }
}

/**
 * @brief  EXTI interrupt handler for both rot A and B pin rising and falling edges.
 */
void io_rotary_rotA_rotB_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_a.pin);
    __HAL_GPIO_EXTI_CLEAR_IT(rot_b.pin);
    rotary_update();
}

/**
 * @brief  EXTI interrupt handler for the push‐switch pin.
 */
void io_rotary_push_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(rot_s.pin);
    if (push_callback)
    {
        push_callback();
    }
}
