#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace app::telemRx
{

// PRECONDITION: ingest() and drain() are called from a SINGLE task
// (currently TaskTelemRx). The rx_time_ms global association is only
// safe under strict ingest->drain ordering on one thread.
// Push freshly-received bytes into the centralized ring buffer.
// rx_time_ms is the RTC time captured immediately after this chunk landed;
// it is associated with any frame whose last byte falls inside this chunk.
void ingest(std::span<const uint8_t> bytes, uint64_t rx_time_ms);

// Drain every complete, validated frame currently in the ring buffer.
void drain();

#ifdef TARGET_TEST
// Diagnostic / test access.
std::size_t ringSize();
void        reset();
#endif
} // namespace app::telemRx
