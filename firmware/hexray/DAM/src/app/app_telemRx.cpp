#include "app_telemRx.hpp"

#include <array>

#include "app_ntp.hpp"
#include "app_crc32.hpp"
#include "io_log.hpp"
#include "io_telemRx.hpp"
#include "util_ringBuffer.hpp"

namespace app::telemRx
{

namespace
{
    constexpr std::size_t ringCapacity = 512;
    constexpr std::size_t maxBodySize  = 100;

    constexpr uint8_t     magic0     = 0xCC;
    constexpr uint8_t     magic1     = 0x33;
    constexpr std::size_t headerSize = 7; // magic(2) + size(1) + crc(4)

    enum class MessageId : uint8_t
    {
        NTP        = 1,
        Remote_NTP = 2,
    };

    util::RingBuffer<uint8_t, ringCapacity> g_ring{};

    // t3 captured by IO immediately after the most recent chunk landed.
    // Associated with any frame extracted on the next drain() pass.
    uint64_t g_last_rx_time_ms = 0;

    // Dispatch the frame to the appropriate handler based on the message id
    void dispatchFrame(std::span<const uint8_t> body, uint64_t rx_time_ms)
    {
        if (body.empty())
            return;

        switch (static_cast<MessageId>(body[0]))
        {
            case MessageId::NTP:
            {
                if (!app::ntp::handleFrameAndTuneRtc(body, rx_time_ms))
                {
                    LOG_ERROR("telemRx: NTP handleFrameAndTuneRtc failed");
                    return;
                }
                break;
            }
            // Remote trigger of transmitNTP when dam is enclosed (button is inaccesible)
            case MessageId::Remote_NTP:
            {
                // Synchronously transmits on the RX task. Safe because the backend's NTP
                // response can't arrive until after this transmit completes (request/response
                // protocol), and Remote_NTP / button NTP are mutually exclusive by policy.
                const auto ntp_result = io::telemRx::transmitNTPStartMsg();
                if (!ntp_result)
                {
                    LOG_ERROR("telemRx: Remote NTP transmit failed");
                    return;
                }
                else
                {
                    app::ntp::recordT0(app::ntp::rtcTimeToMs(*ntp_result));
                    LOG_INFO("telemRx: Remote NTP transmit successful");
                    LOG_INFO(
                        "Remote NTP Msg! at time: %02u:%02u:%02u.%03lu", ntp_result->hours, ntp_result->minutes,
                        ntp_result->seconds, static_cast<unsigned long>(999 - ntp_result->subseconds));
                }
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
            if (g_ring.peek(0) == magic0 && g_ring.peek(1) == magic1)
                break;
            g_ring.discard(1);
        }
        if (g_ring.size() < headerSize)
            return false;

        const uint8_t  body_size    = g_ring.peek(2);
        const uint32_t expected_crc = g_ring.peekU32Le(3);

        if (body_size == 0 || static_cast<std::size_t>(body_size) > maxBodySize)
        {
            g_ring.discard(1); // bad header: slide one byte and try again
            return true;
        }

        if (g_ring.size() < headerSize + body_size)
            return false; // wait for more bytes

        // CRC directly against ring memory so a bad frame costs no body copy.
        const auto [s1, s2] = g_ring.contiguousRange(headerSize, body_size);
        if (s1.size() + s2.size() != body_size)
            return false;

        uint32_t c = app::crc32::init();
        c          = app::crc32::update(c, s1.data(), s1.size());
        if (!s2.empty())
            c = app::crc32::update(c, s2.data(), s2.size());

        if (app::crc32::finalize(c) != expected_crc)
        {
            LOG_ERROR("telemRx: CRC mismatch, resyncing");
            g_ring.discard(1); // resync one byte at a time
            return true;
        }

        std::array<uint8_t, maxBodySize> body_buf{};
        std::span<uint8_t>               body{ body_buf.data(), body_size };
        (void)g_ring.copyOut(headerSize, body);

        g_ring.discard(headerSize + body_size);
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

// Read bytes from the ring buffer until end of valid message
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
