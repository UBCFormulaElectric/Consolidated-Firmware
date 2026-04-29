#pragma once

#include <cstdint>
#include <span>

namespace io::crc
{

/**
 * Create the mutex serializing access to the shared STM32 CRC peripheral.
 * Must be called once at startup, before any task that uses calculatePayloadCrc is created.
 */
void init();

/**
 * Calculate telemetry payload CRC.
 * Pads bytes to 32-bit words for the hardware CRC peripheral and applies protocol inversion.
 * Thread-safe: serialized via an internal mutex, since the STM32 CRC peripheral has a single
 * hardware state machine shared across all callers.
 */
[[nodiscard]] uint32_t calculatePayloadCrc(std::span<const uint8_t> payload);

} // namespace io::crc
