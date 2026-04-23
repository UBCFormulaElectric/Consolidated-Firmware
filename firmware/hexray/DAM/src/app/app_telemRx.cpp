#include "app_telemRx.hpp"

#include <array>
#include <cstring>

#include "app_ntp.hpp"
#include "io_crc.hpp"
#include "io_log.hpp"
#include "io_rtc.hpp"
#include "util_ringBuffer.hpp"

namespace app::telemRx
{

namespace
{
util::RingBuffer<uint8_t, kRingCapacity> g_ring{};

// t3 captured by IO immediately after the most recent chunk landed.
// Associated with any frame extracted on the next drain() pass.
uint64_t g_last_rx_time_ms = 0;

uint32_t readU32Le(std::size_t offset)
{
    uint32_t v = 0;
    v |= static_cast<uint32_t>(g_ring.peek(offset + 0)) << 0;
    v |= static_cast<uint32_t>(g_ring.peek(offset + 1)) << 8;
    v |= static_cast<uint32_t>(g_ring.peek(offset + 2)) << 16;
    v |= static_cast<uint32_t>(g_ring.peek(offset + 3)) << 24;
    return v;
}

void dispatchFrame(std::span<const uint8_t> body, uint64_t rx_time_ms)
{
    if (body.empty())
        return;

    switch (static_cast<MessageId>(body[0]))
    {
        case MessageId::Ntp:
        {
            io::rtc::Time now{};
            const auto    t = io::rtc::get_time(now);
            if (!t)
            {
                LOG_ERROR("telemRx: RTC get_time failed");
                return;
            }
            const auto new_ms = app::ntp::handleFrame(body, rx_time_ms, app::ntp::rtcTimeToMs(now));
            if (!new_ms)
            {
                LOG_ERROR("telemRx: NTP body parse failed");
                return;
            }
            io::rtc::Time tuned = app::ntp::msToRtcTime(*new_ms);
            tuned.subseconds    = 0;
            const auto set_res  = io::rtc::set_time(tuned);
            if (!set_res)
            {
                LOG_ERROR("telemRx: RTC set_time failed");
                return;
            }
            LOG_INFO(
                "Tuned RTC! New Time: %02u:%02u:%02u.%03lu", tuned.hours, tuned.minutes, tuned.seconds,
                static_cast<unsigned long>(999 - tuned.subseconds));
            break;
        }
        default:
            LOG_WARN("telemRx: unknown message id 0x%02X", body[0]);
            break;
    }
}

// Try to extract one frame. Returns true if any byte was consumed (frame
// taken or one byte discarded for resync); false when the ring needs more
// data before we can make progress.
bool tryParseFrame()
{
    // Hunt for magic.
    while (g_ring.size() >= 2)
    {
        if (g_ring.peek(0) == kMagic0 && g_ring.peek(1) == kMagic1)
            break;
        g_ring.discard(1);
    }
    if (g_ring.size() < kHeaderSize)
        return false;

    const uint8_t  body_size    = g_ring.peek(2);
    const uint32_t expected_crc = readU32Le(3);

    if (body_size == 0 || static_cast<std::size_t>(body_size) > kMaxBodySize)
    {
        g_ring.discard(1); // bad header: slide one byte and try again
        return true;
    }

    if (g_ring.size() < kHeaderSize + body_size)
        return false; // wait for more bytes

    std::array<uint8_t, kMaxBodySize> body_buf{};
    std::span<uint8_t>                body{ body_buf.data(), body_size };
    if (!g_ring.copyOut(kHeaderSize, body))
        return false;

    if (io::crc::calculatePayloadCrc(body) != expected_crc)
    {
        LOG_ERROR("telemRx: CRC mismatch, resyncing");
        g_ring.discard(1); // resync one byte at a time
        return true;
    }

    g_ring.discard(kHeaderSize + body_size);
    dispatchFrame(body, g_last_rx_time_ms);
    return true;
}
} // namespace

void ingest(std::span<const uint8_t> bytes, uint64_t rx_time_ms)
{
    g_last_rx_time_ms = rx_time_ms;
    if (!g_ring.push(bytes))
    {
        // Ring overflow: drop oldest bytes to make room. Better than silent
        // truncation of the new chunk — newer data is more useful for resync.
        const std::size_t need = bytes.size() > g_ring.free() ? bytes.size() - g_ring.free() : 0;
        g_ring.discard(need);
        (void)g_ring.push(bytes);
        LOG_WARN("telemRx: ring overflow, dropped %u bytes", static_cast<unsigned>(need));
    }
}

void drain()
{
    while (tryParseFrame())
    {
    }
}

std::size_t ringSize()
{
    return g_ring.size();
}

void reset()
{
    g_ring.clear();
    g_last_rx_time_ms = 0;
}

} // namespace app::telemRx
