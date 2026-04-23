#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace app::telemRx
{

inline constexpr std::size_t kRingCapacity = 512;
inline constexpr std::size_t kChunkSize    = 32;
inline constexpr std::size_t kMaxBodySize  = 256;

inline constexpr uint8_t     kMagic0     = 0xCC;
inline constexpr uint8_t     kMagic1     = 0x33;
inline constexpr std::size_t kHeaderSize = 7; // magic(2) + size(1) + crc(4)

enum class MessageId : uint8_t
{
    Ntp = 1,
};

// Push freshly-received bytes into the centralized ring buffer.
// rx_time_ms is the RTC time captured immediately after this chunk landed;
// it is associated with any frame whose last byte falls inside this chunk.
void ingest(std::span<const uint8_t> bytes, uint64_t rx_time_ms);

// Drain every complete, validated frame currently in the ring buffer.
void drain();

// Diagnostic / test access.
std::size_t ringSize();
void        reset();

} // namespace app::telemRx
