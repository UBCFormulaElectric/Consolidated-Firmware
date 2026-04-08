#include "io_telemRx.hpp"
#include "hw_uarts.hpp"
#include "hw_uart.hpp"
#include "io_telemMessage.hpp"
#include "io_rtc.hpp"
#include <portmacro.h>
#include <stdint.h>
#include <util_errorCodes.h>
#include <cstring>

constexpr uint32_t PREDIV_S = 999;
constexpr uint64_t MS_PER_DAY = 86400000ULL;

static NTPTimestamps ntpTimestamps;

void io_telemRx()
{
    pollForRadioMessages();
}

// void receiveTest()
// {
//     std::array<uint8_t, 8> buffer{};
//     std::span<uint8_t>     rx(buffer);

//     auto result = _900k_uart.receive(rx); // 1 second timeout

//     if (!result.has_value())
//     {
//         LOG_ERROR("UART receive failed! Error: %d\n", static_cast<int>(result.error()));
//         return;
//     }

//     LOG_INFO("UART receive succeeded. Data:\n");
//     for (auto b : buffer)
//     {
//         LOG_INFO("0x%02X ", b);
//     }
// }

// Send message to backend through radio to get t1,t2. Called periodically
void transmitNTPStartMsg(void)
{
    // Take note of the sending time (t0).
    io::rtc::Time t0;
    if (!io::rtc::get_time(t0))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }
    ntpTimestamps.t0 = RtcTimeToMs(t0);

    const io::telemMessage::NTPMsg ntp_msg = io::telemMessage::NTPMsg();
    if (!_900k_uart.transmit(
            std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&ntp_msg), ntp_msg.wireSize() }))
    {
        LOG_ERROR("Failed to transmit NTP message");
        return;
    }

    // --------------------------- Debug message
    LOG_INFO("Sent NTP Msg! at time: %02u:%02u:%02u.%03lu", t0.hours, t0.minutes, t0.seconds, (unsigned long)(999 - t0.subseconds));
}

void pollForRadioMessages(void)
{
    // Structure: First 2 bytes is magic bytes, 3rd is size of the body, remaining 4 is CRC
    uint8_t            headerData[7];
    std::span<uint8_t> rxBufferHeader(headerData, 7);
    auto               result = _900k_uart.receive(rxBufferHeader);
    if (!result)
    {
        LOG_ERROR("Could not get rxBufferHeader, error %d", result.error());
        return;
    }

    // Keep shifting until magic bytes found
    while (true)
    {
        if (rxBufferHeader[0] == 0xCC && rxBufferHeader[1] == 0x33)
            break;

        for (std::size_t i = 0; i < rxBufferHeader.size() - 1; i++)
            rxBufferHeader[i] = rxBufferHeader[i + 1];

        // Read 1 new byte into last position
        if (!_900k_uart.receive(std::span<uint8_t>{ &rxBufferHeader[6], 1 }))
        {
            LOG_ERROR("Could not read 1 new header byte");
            return;
        }
    }

    LOG_INFO(
        "Header: %02X %02X %02X %02X %02X %02X %02X", rxBufferHeader[0], rxBufferHeader[1], rxBufferHeader[2],
        rxBufferHeader[3], rxBufferHeader[4], rxBufferHeader[5], rxBufferHeader[6]);

    // TODO check CRC here

    // Read rest of packet (contains t1, t2) using size given to you
    uint8_t            size = rxBufferHeader[3];
    uint8_t            bodyData[256];
    std::span<uint8_t> rxBufferBody(bodyData, size);

    io::rtc::Time t3; // declare it here to not waste time when capturing t3

    if (!_900k_uart.receive(rxBufferBody))
    {
        LOG_ERROR("Could not get rxBufferBody");
        return;
    }

    // Take note of t3 (receiving time)
    if (!io::rtc::get_time(t3))
    {
        LOG_ERROR("Could not get RTC time for t3");
        return;
    }
    ntpTimestamps.t3 = RtcTimeToMs(t3);

    // Parse t1 and t2 from rxBufferBody.
    parseNTPPacketBody(rxBufferBody);

    // ---------------- Debug Prints -------------------
    io::rtc::Time t = MsToRtcTime(ntpTimestamps.t0);
    LOG_INFO("Noted t0:    %02u:%02u:%02u.%03lu", t.hours, t.minutes, t.seconds, (unsigned long)(999 - t.subseconds));
    io::rtc::Time tt = MsToRtcTime(ntpTimestamps.t1);
    LOG_INFO("Received t1: %02u:%02u:%02u.%03lu", tt.hours, tt.minutes, tt.seconds, (unsigned long)(999 - tt.subseconds));
    io::rtc::Time ttt = MsToRtcTime(ntpTimestamps.t2);
    LOG_INFO("Received t2: %02u:%02u:%02u.%03lu", ttt.hours, ttt.minutes, ttt.seconds, (unsigned long)(999 - ttt.subseconds));
    io::rtc::Time tttt = MsToRtcTime(ntpTimestamps.t3);
    LOG_INFO("Noted t3:    %02u:%02u:%02u.%03lu", tttt.hours, tttt.minutes, tttt.seconds, (unsigned long)(999 - tttt.subseconds));

    // Tune RTC with collected t0, t1, t2, t3
    tuneRTC();
}

void parseNTPPacketBody(std::span<uint8_t> body)
{
    if (body.size() < 17)
        return; // Invalid packet body size

    uint64_t messageID = body[0];
    if (messageID != 1)
        return; // Received non-ntp message

    // Body is 17 bytes: 1 byte header + 8 bytes t1 + 8 bytes t2
    uint64_t t1;
    uint64_t t2;

    std::memcpy(&t1, &body[1], sizeof(uint64_t));
    std::memcpy(&t2, &body[9], sizeof(uint64_t));

    ntpTimestamps.t1 = t1 % MS_PER_DAY;
    ntpTimestamps.t2 = t2 % MS_PER_DAY;
}

void tuneRTC(void)
{
    // Calculate the offset theta using the formula
    int64_t theta = ((int64_t)(ntpTimestamps.t1 - ntpTimestamps.t0) +
                     (int64_t)(ntpTimestamps.t2 - ntpTimestamps.t3)) / 2;

    io::rtc::Time currTime;
    if (!io::rtc::get_time(currTime))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }

    int64_t currMs = static_cast<int64_t>(RtcTimeToMs(currTime));
    int64_t newMs  = currMs + theta;

    if (newMs < 0)
        newMs = 0;

    io::rtc::Time newRtcTime = MsToRtcTime(static_cast<uint64_t>(newMs));

    // extract the ms
    //uint32_t ms = PREDIV_S - newRtcTime.subseconds;

    // delay the ms amount of time (this calls os_delay so it is non-blocking)
    //if (theta > 0) // only delay when moving forward - we want to apply negative adjustments asap
        //io::time::delay(ms); 

    // round the newRtcTime ms field up to the nearest second
    // this needs to be done because io::rtc::set_time() doesn't use the subseconds field to set rtc time
    newRtcTime.subseconds = 0;

    // Tune the RTC
    if (!io::rtc::set_time(newRtcTime))
        LOG_ERROR("Failed to tune RTC");

    // ------------- Debug message -------------
    LOG_INFO(
        "Tuned RTC! New Time: %02u:%02u:%02u.%03lu", newRtcTime.hours, newRtcTime.minutes, newRtcTime.seconds,
        (unsigned long)(999 - newRtcTime.subseconds));
}

uint64_t RtcTimeToMs(io::rtc::Time t)
{
    // Convert subseconds to ms using inverted relation
    uint32_t ms_part = PREDIV_S - t.subseconds;
    return static_cast<uint64_t>(t.hours) * 3600000ULL + static_cast<uint64_t>(t.minutes) * 60000ULL +
           static_cast<uint64_t>(t.seconds) * 1000ULL + static_cast<uint64_t>(ms_part);
}

io::rtc::Time MsToRtcTime(uint64_t ms)
{
    io::rtc::Time t{};

    ms %= 86400000ULL;
    t.hours = static_cast<uint8_t>(ms / 3600000ULL);
    ms %= 3600000ULL;
    t.minutes = static_cast<uint8_t>(ms / 60000ULL);
    ms %= 60000ULL;

    t.seconds             = static_cast<uint8_t>(ms / 1000ULL);
    uint32_t ms_remainder = static_cast<uint32_t>(ms % 1000ULL);

    // Store inverted for RTC
    t.subseconds = PREDIV_S - ms_remainder;
    return t;
}
