/**
 *
 * This is the 1-Wire equivalent of hw_spis.hpp: it moves bits and knows nothing about what is on
 * the other end. All DS18B20 semantics live in io_emeter. Implemented in hw_onewire.cpp against
 * two GPIOs -- an open-drain DQ line and a push-pull gate driving the strong-pullup MOSFET.
 *
 * Timing rules the implementation has to honour (DS18B20 datasheet, Figure 16):
 *   - a time slot is 60-120us, and a read slot must be sampled within 15us of the falling edge;
 *   - each *bit* therefore runs inside a critical section. Between bits the bus idles high and
 *     "infinite recovery time" is allowed (pg. 10), so the scheduler may run freely there. Do not
 *     widen the critical section to a whole byte -- that is 560us of interrupts-off and would
 *     wreck the 1kHz task;
 *   - the reset pulse is the exception: tRSTL must be >= 480us, but under parasite power must stay
 *     under 960us or every device on the bus power-on resets (Note 13). Its low phase needs its own
 *     critical section.
 */
#pragma once

#include "util_errorCodes.hpp"

#include <cstdint>
#include <span>

namespace hw::onewire
{
/**
 * Drives a reset pulse and listens for the presence pulse (Figure 15).
 * @return {} if at least one device answered, ErrorCode::TIMEOUT if the bus stayed high.
 */
[[nodiscard]] result<void> reset();

void writeByte(uint8_t byte);
void writeBytes(std::span<const uint8_t> bytes);

[[nodiscard]] uint8_t readByte();
void                  readBytes(std::span<uint8_t> bytes);

/**
 * A single read time slot. Read Power Supply [0xB4] answers with one bit, not a byte.
 * @return the bus level sampled in the slot: true = released high, false = pulled low.
 */
[[nodiscard]] bool readBit();

/**
 * Writes @p byte and switches the bus to the strong pullup MOSFET, both inside one critical
 * section.
 *
 * This exists as a single call because Convert T [0x44] gives the master 10us (max) to get the
 * strong pullup on (pg. 7). That is far too tight to return up to io, cross an RTOS scheduling
 * boundary, and come back down -- so the write and the switch cannot be separated.
 *
 * The caller owns the bus until strongPullupOff(): no other 1-Wire traffic may happen while the
 * pullup is driving.
 */
void writeByteThenStrongPullup(uint8_t byte);

/** Releases the strong pullup, handing the bus back to the external 4.7k resistor. */
void strongPullupOff();
} // namespace hw::onewire
