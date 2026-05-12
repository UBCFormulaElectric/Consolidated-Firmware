#include "app_telemRx.hpp"

#include <array>

#include "app_ntp.hpp"
#include "app_crc32.hpp"
#include "io_log.hpp"
#include "io_rtc.hpp"
#include "io_telemMessage.hpp"
#include "io_telemQueue.hpp"
#include "util_ringBuffer.hpp"

namespace app::telemRx
{

namespace
{
    // Largest incoming telem frame from the Rust backend (NTP): 7B header + 17B body.
    constexpr std::size_t maxIncomingFrameSize = 24;
    constexpr std::size_t ringCapacity         = maxIncomingFrameSize * 22; // 528 bytes
    constexpr std::size_t maxBodySize          = 100;

    constexpr uint8_t     magic0     = 0xCC;
    constexpr uint8_t     magic1     = 0x33;
    constexpr std::size_t headerSize = 7; // magic(2) + size(1) + crc(4)

    enum class MessageId : uint8_t
    {
        NTP        = 1,
        Remote_NTP = 2,
    };

    // TaskTelemRx is the only producer (via ingest); TaskTelemParse is
    // the only consumer (via drain). Overflow policy is drop-incoming, so the
    // producer never touches queued data.
    util::RingBuffer<uint8_t, ringCapacity> g_ring{};

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
            case MessageId::Remote_NTP:
            {
                if (app::ntp::isNtpInProgress())
                {
                    LOG_WARN("telemRx: NTP already in progress, ignoring Remote_NTP");
                    return;
                }
                app::ntp::setNtpInProgress();
                const auto push_result =
                    telem_tx_queue.push(io::telemMessage::TelemQueueEntry(io::telemMessage::NTPMsg{}));
                if (!push_result)
                {
                    LOG_ERROR("telemRx: Failed to enqueue Remote NTP: %d", static_cast<int>(push_result.error()));
                    app::ntp::clearNtpInProgress();
                    return;
                }
                LOG_INFO("telemRx: Remote NTP enqueued");
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
        // Drop-incoming overflow means the producer never moves head_, so the
        // size() check above guarantees this range is fully populated.
        const auto [s1, s2] = g_ring.contiguousRange(headerSize, body_size);

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

        // validated message from here
        std::array<uint8_t, maxBodySize> body_buf{};
        std::span<uint8_t>               body{ body_buf.data(), body_size };
        (void)g_ring.copyOut(headerSize, body);
        g_ring.discard(headerSize + body_size);

        // Capture t3 once we know we have a real frame. This adds parser-
        // scheduling latency to t3, but avoids any producer/consumer sharing
        // of the timestamp.
        io::rtc::Time t3_now{};
        uint64_t      t3_ms = 0;
        if (io::rtc::get_time(t3_now))
        {
            t3_ms = app::ntp::rtcTimeToMs(t3_now);
        }
        else
        {
            LOG_ERROR("telemRx: RTC get_time failed while capturing t3");
        }

        dispatchFrame(body, t3_ms);
        return true;
    }
} // namespace

void ingest(std::span<const uint8_t> bytes)
{
    if (!g_ring.push(bytes))
    {
        LOG_WARN("telemRx: ring full, dropped %u incoming bytes", static_cast<unsigned>(bytes.size()));
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
}

} // namespace app::telemRx
