#include "io_telemRx.hpp"
#include "io_telemUart.hpp"
#include "io_telemMessage.hpp"
#include "io_rtc.hpp"
#include <stdint.h>
#include <util_errorCodes.h>

constexpr uint32_t PREDIV_S = 999;

static NTPTimestamps ntpTimestamps;

void io_telemRx() // Make this noreturn
{
    pollForRadioMessages();
}

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
    if (not io::telemUart::transmitIt(
            std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&ntp_msg), ntp_msg.wireSize() }))
    {
        LOG_ERROR("Failed to transmit NTP message");
        return;
    }
}

void pollForRadioMessages(void)
{
    // Structure: First 2 bytes is magic bytes, 3rd is size of the body, remaining 4 is CRC

    uint8_t headerData[7]; 
    std::span<uint8_t> rxBufferHeader(headerData, 7);
    auto result = io::telemUart::receiveIt(rxBufferHeader);
    if (!result)
        return;

    // Keep shifting until magic bytes found
    while (true)
    {
        if (rxBufferHeader[0] == 0xCC && rxBufferHeader[1] == 0x33)
            break;

        LOG_INFO("magic bytes not found, searching ...");
        
        for (std::size_t i = 0; i < rxBufferHeader.size() - 1; i++)
            rxBufferHeader[i] = rxBufferHeader[i + 1];

        // Read 1 new byte into last position
        result = io::telemUart::receiveIt(&rxBufferHeader[6], 1);
        if (!result)
            return;
    }

    LOG_INFO(
        "Header: %02X %02X %02X %02X %02X %02X %02X", rxBufferHeader[0], rxBufferHeader[1], rxBufferHeader[2],
        rxBufferHeader[3], rxBufferHeader[4], rxBufferHeader[5], rxBufferHeader[6]);

    // TODO check CRC here

    // Read rest of packet (contains t1, t2) using size given to you
    uint8_t size = rxBufferHeader[3];
    uint8_t bodyData[256];
    result = io::telemUart::receiveIt(std::span<uint8_t>(&rxBufferHeader[6], 1));
    
    if (!io::telemUart::receiveIt(rxBufferBody) != 0)
        return;

    // Take note of t3 (receiving time)
    io::rtc::Time t3;
    if (!io::rtc::get_time(t3))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }
    ntpTimestamps.t3 = RtcTimeToMs(t3);

    // Parse t1 and t2 from rxBufferBody.
    parseNTPPacketBody(rxBufferBody);

    // Tune RTC with collected t1, t2, t3, t4.
    tuneRTC();
    
}

void parseNTPPacketBody(std::span<uint8_t> body)
{
    if (body.size() < 17)
        return; // Invalid packet body size

    uint64_t messageID = body[0];
    if (messageID != 2)
        return; // Received non-ntp message

    // Body is 17 bytes: 1 byte header + 8 bytes t1 + 8 bytes t2
    uint64_t t1 = 0;
    uint64_t t2 = 0;

    // Parse t1 (bytes 1–8) - little endian
    for (size_t i = 0; i < 8; i++) {
        t1 |= static_cast<uint64_t>(body[1 + i]) << (8 * i);
    }

    // Parse t2 (bytes 9–16) - little endian
    for (size_t i = 0; i < 8; i++) {
        t2 |= static_cast<uint64_t>(body[9 + i]) << (8 * i);
    }

    ntpTimestamps.t1 = t1;
    ntpTimestamps.t2 = t2;
}

void tuneRTC(void)
{
    // Calculate the offset theta using the formula
   int64_t theta = ((int64_t)ntpTimestamps.t1 - (int64_t)ntpTimestamps.t0 +
                    (int64_t)ntpTimestamps.t2 - (int64_t)ntpTimestamps.t3) / 2;

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
    uint32_t ms = PREDIV_S - newRtcTime.subseconds;

    // round the ms field up to the nearest second
    newRtcTime.subseconds = 0;

    // delay the ms amount of time (this calls os_delay so it is non-blocking)
    if (theta > 0) // only delay when moving forward - we want to apply negative adjustments asap
        io::time::delay(ms);

    // Tune the RTC
    if (!io::rtc::set_time(newRtcTime))
        LOG_ERROR("Failed to tune RTC!");

    LOG_INFO("Tuned RTC!"); // Debug msg
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
