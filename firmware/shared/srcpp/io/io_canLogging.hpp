#pragma once

// Raw-CAN-frame on-disk framing used by logfs SD card logs.
//
// This is the C++ counterpart of firmware/shared/src/io/io_canLogging.c:108-138.
// The wire format MUST stay byte-identical across implementations so the same
// Python decoder (firmware/logfs/python/logfs/can_logger.py) can read SD cards
// from any board.
//
// Per-message layout:
//   0xBA            magic        (1 B)
//   dlc             length       (1 B)
//   timestamp       low 16 b ms  (2 B, boot-monotonic)
//   std_id          CAN ID       (4 B)
//   data[0..dlc]    payload
//   crc8            over header + payload (1 B)
//
// Only the serializer lives here. Owning the file handle, queue, path, and
// bootcount is left to each board because those touch board-specific
// `fs` / RTOS APIs.

#include <array>
#include <cstddef>
#include <cstdint>

#include "io_canMsg.hpp"

namespace io::canLogging
{
inline constexpr size_t HEADER_BYTES      = 8; // magic + dlc + ts16 + id
inline constexpr size_t CRC_BYTES         = 1; // crc8 trailer
inline constexpr size_t MAX_ENCODED_BYTES = HEADER_BYTES + io::CAN_PAYLOAD_BYTES + CRC_BYTES;

using EncodeBuf = std::array<uint8_t, MAX_ENCODED_BYTES>;

// Serialize `msg` into `out` using the framing above.
// Returns the number of bytes written into `out` (always
// HEADER_BYTES + msg.dlc + CRC_BYTES).
size_t encode(const io::CanMsg &msg, EncodeBuf &out);
} // namespace io::canLogging
