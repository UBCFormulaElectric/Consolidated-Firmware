#pragma once

#include <cstdint>
#include <span>

namespace io::crc
{

/**
 * Calculate telemetry payload CRC.
 * This pads bytes to 32-bit words for the hardware CRC peripheral and applies protocol inversion.
 */
[[nodiscard]] uint32_t calculatePayloadCrc(std::span<const uint8_t> payload);

} // namespace io::crc
