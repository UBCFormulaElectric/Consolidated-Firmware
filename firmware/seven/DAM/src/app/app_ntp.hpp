#pragma once

#include <cstdint>

namespace app::ntp
{

struct Timestamps
{
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
};

#ifdef TARGET_TEST
// Read the most recently recorded timestamps. Test-only diagnostic accessor.
const Timestamps &timestamps();
#endif

// Apply an NTP correction to the RTC using the four protocol timestamps
// (t0 captured by tryBeginAndCaptureT0; t1/t2 supplied by the caller from a
// parsed NTP reply; t3 supplied by the caller at frame-receipt time).
// Returns false if any stage fails.
bool handleFrameAndTuneRtc(uint64_t t1, uint64_t t2, uint64_t t3_ms);

// Called by the telem TX worker just before transmitting an NTPMsg. Atomically
// claims the in-flight slot and captures T0 from the RTC. Returns false if an
// exchange is already in progress (dismiss this request) or the RTC read failed.
// On any failure the slot is left clear.
bool tryBeginAndCaptureT0();

} // namespace app::ntp
