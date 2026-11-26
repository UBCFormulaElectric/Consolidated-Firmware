#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace app::telemRx
{

// CONCURRENCY: ingest() and drain() form a single-producer / single-consumer
// pair. ingest() is called from the UART rx task (TaskTelemRx); drain() is
// called from the parser task (TaskTelemParse). The ring's producer end
// (tail) and consumer end (head) are touched by their respective tasks only;
// overflow policy is drop-incoming so the producer never mutates queued data.
// Calling ingest() from more than one task, or drain() from more than one
// task, is NOT supported.
//
// Push freshly-received bytes into the centralized ring buffer. The receive
// timestamp (t3) is captured by the consumer when a full frame parses, so the
// producer does not pass it through.
void ingest(std::span<const uint8_t> bytes);

// Drain every complete, validated frame currently in the ring buffer.
// Returns when the ring contains no more parseable bytes; the caller is
// expected to block on a notification from the producer before re-invoking.
void drain();

#ifdef TARGET_TEST
// Diagnostic / test access.
std::size_t ringSize();
void        reset();
#endif
} // namespace app::telemRx
