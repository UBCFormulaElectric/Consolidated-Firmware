#include "io_telemRx.hpp"
#include "app_ntp.hpp"
#include "hw_uarts.hpp"
#include "hw_uart.hpp"
#include "io_telemMessage.hpp"
#include "io_rtc.hpp"
#include "io_crc.hpp"
#include <portmacro.h>
#include <stdint.h>
#include <util_errorCodes.h>
#include <cstring>

static app::ntp::Timestamps ntpTimestamps;

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
void io::telemRx::transmitNTPStartMsg(void)
{
    // Take note of the sending time (t0).
    io::rtc::Time t0;
    if (!io::rtc::get_time(t0))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }
    ntpTimestamps.t0 = app::ntp::rtcTimeToMs(t0);

    const io::telemMessage::NTPMsg ntp_msg = io::telemMessage::NTPMsg();
    if (!_900k_uart.transmit(
            std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&ntp_msg), ntp_msg.wireSize() }))
    {
        LOG_ERROR("Failed to transmit NTP message");
        return;
    }

    // --------------------------- Debug message
    LOG_INFO(
        "Sent NTP Msg! at time: %02u:%02u:%02u.%03lu", t0.hours, t0.minutes, t0.seconds,
        (unsigned long)(999 - t0.subseconds));
}

void io::telemRx::pollForRadioMessages(void)
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

    // Read rest of packet (contains t1, t2) using size given to you
    uint8_t size = rxBufferHeader[2];

    if (size == 0)
    {
        LOG_ERROR("Invalid payload size: 0");
        return;
    }

    uint8_t            bodyData[256];
    std::span<uint8_t> rxBufferBody(bodyData, size);

    io::rtc::Time t3; // declare it here to not waste time when capturing t3

    if (!_900k_uart.receive(rxBufferBody))
    {
        LOG_ERROR("Could not get rxBufferBody");
        return;
    }

    // Take note of t3 (receiving time) immediately after receive
    if (!io::rtc::get_time(t3))
    {
        LOG_ERROR("Could not get RTC time for t3");
        return;
    }
    ntpTimestamps.t3 = app::ntp::rtcTimeToMs(t3);

    // CRC validation: expected CRC is little-endian in header bytes [3..6]
    uint32_t expected_crc;
    std::memcpy(&expected_crc, &rxBufferHeader[3], sizeof(uint32_t));
    if (expected_crc != io::crc::calculatePayloadCrc(rxBufferBody))
    {
        LOG_ERROR("CRC mismatch");
        return;
    }

    // Parse t1 and t2 from rxBufferBody.
    if (!app::ntp::parseNTPPacketBody(rxBufferBody, ntpTimestamps))
    {
        LOG_ERROR("Failed to parse NTP packet body");
        return;
    }

    // ---------------- Debug Prints -------------------
    io::rtc::Time t = app::ntp::msToRtcTime(ntpTimestamps.t0);
    LOG_INFO("Noted t0:    %02u:%02u:%02u.%03lu", t.hours, t.minutes, t.seconds, (unsigned long)(999 - t.subseconds));
    io::rtc::Time tt = app::ntp::msToRtcTime(ntpTimestamps.t1);
    LOG_INFO(
        "Received t1: %02u:%02u:%02u.%03lu", tt.hours, tt.minutes, tt.seconds, (unsigned long)(999 - tt.subseconds));
    io::rtc::Time ttt = app::ntp::msToRtcTime(ntpTimestamps.t2);
    LOG_INFO(
        "Received t2: %02u:%02u:%02u.%03lu", ttt.hours, ttt.minutes, ttt.seconds,
        (unsigned long)(999 - ttt.subseconds));
    io::rtc::Time tttt = app::ntp::msToRtcTime(ntpTimestamps.t3);
    LOG_INFO(
        "Noted t3:    %02u:%02u:%02u.%03lu", tttt.hours, tttt.minutes, tttt.seconds,
        (unsigned long)(999 - tttt.subseconds));

    // Tune RTC with collected t0, t1, t2, t3
    int64_t theta = app::ntp::computeOffset(ntpTimestamps);

    io::rtc::Time currTime;
    if (!io::rtc::get_time(currTime))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }

    int64_t currMs = static_cast<int64_t>(app::ntp::rtcTimeToMs(currTime));
    int64_t newMs  = currMs + theta;

    if (newMs < 0)
        newMs = 0;

    io::rtc::Time newRtcTime = app::ntp::msToRtcTime(static_cast<uint64_t>(newMs));
    newRtcTime.subseconds    = 0;

    if (!io::rtc::set_time(newRtcTime))
        LOG_ERROR("Failed to tune RTC");

    LOG_INFO(
        "Tuned RTC! New Time: %02u:%02u:%02u.%03lu", newRtcTime.hours, newRtcTime.minutes, newRtcTime.seconds,
        (unsigned long)(999 - newRtcTime.subseconds));
}
