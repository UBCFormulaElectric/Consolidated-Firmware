#pragma once

using RotaryCallback = void (*)();

namespace io::rotary
{
/**
 * @brief  Initialize the rotary encoder module.
 *         Reads the initial levels of A and B pins to set prev state.
 */
void init();
/**
 * @brief  Set a callback to be called on each cw event.
 * @param  cb  Function pointer called when encoder turns cw.
 */
void setClockwiseCallback(RotaryCallback cb);
/**
 * @brief  Set a callback to be called on each ccw event.
 * @param  cb  Function pointer called when encoder turns ccw.
 */
void setCounterClockwiseCallback(RotaryCallback cb);
/**
 * @brief  Set a callback to be called on each push event.
 * @param  cb  Function pointer called when encoder pushes.
 */
void setPushCallback(RotaryCallback cb);
/**
 * @brief  EXTI interrupt handler for both rot A and B pin rising and falling edges.
 */
void rotA_rotB_IRQHandler();
/**
 * @brief  EXTI interrupt handler for the push‐switch pin.
 */
void push_IRQHandler();
} // namespace io::rotary