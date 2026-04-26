#include "io_telemRx.hpp"
#include "hw_uart.hpp"
#include "hw_uarts.hpp"
#include "io_telemMessage.hpp"
#include "io_rtc.hpp"
#include <stdint.h>
#include <util_errorCodes.h>
#include "io_telemUart.hpp"
static NTPTimestamps ntpTimestamps;

void io_telemRx()
{
    pollForRadioMessages();
}

// Send message to backend through radio to get t1,t2
void transmitNTPStartMsg(void)
{
    // Take note of the sending time (t0).
    io::rtc::Time t0;
    if (!io::rtc::get_time(t0))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }
    ntpTimestamps.t0 = t0;

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
    uint8_t rxBufferHeader[7];

    const auto result = io::telemUart::receiveIt(std::span<uint8_t>{ rxBufferHeader, 7 });
    if (!result)
    {
        LOG_ERROR("Failed to receive radio message");
        return;
    }
    // Keep shifting until magic bytes found
    while (true)
    {
        if (rxBufferHeader[0] == 0xCC && rxBufferHeader[1] == 0x33)
            break;

        LOG_INFO("magic bytes not found, searching ...");

        for (int i = 0; i < 6; i++)
        {
            rxBufferHeader[i] = rxBufferHeader[i + 1];
        }

        // Read 1 new byte into last position
        if (not io::telemUart::receiveIt(std::span<uint8_t>{ &rxBufferHeader[6], 1 }))
            return;
    }

    LOG_INFO(
        "Header: %02X %02X %02X %02X %02X %02X %02X", rxBufferHeader[0], rxBufferHeader[1], rxBufferHeader[2],
        rxBufferHeader[3], rxBufferHeader[4], rxBufferHeader[5], rxBufferHeader[6]);

    // TODO check CRC here

    // Read rest of packet (contains t1, t2) using size given to you
    uint8_t size = rxBufferHeader[3];
    // uint8_t rxBufferBody[size];

    // if (not io::telemUart::receivePoll(std::span<uint8_t>{ rxBufferBody, size }))
    // {
    //     LOG_ERROR("Failed to receive radio message body");
    //     return;
    // }
    // TODO u cant assign rxBufferBody[size] because it is not a static array

    // Take note of t3 (receiving time)
    io::rtc::Time t3;
    if (!io::rtc::get_time(t3))
    {
        LOG_ERROR("Could not get RTC time");
        return;
    }
    ntpTimestamps.t3 = t3;

    // Parse t1 and t2 from rxBufferBody.
    // parseNTPPacketBody(rxBufferBody);

    // Tune RTC.
}

void parseNTPPacketBody(uint8_t rxBufferBody[])
{
    uint64_t messageID = rxBufferBody[0];

    uint64_t t1 = 0;
    uint64_t t2 = 0;

    if (messageID == 1)
    {
        // Bytes [8:1] to t1
        for (int i = 0; i < 8; i++)
        {
            t1 |= ((uint64_t)rxBufferBody[i + 1]) << (8 * i);
        }

        // Bytes [16:9] to t2
        for (int i = 0; i < 8; i++)
        {
            t2 |= ((uint64_t)rxBufferBody[i + 9]) << (8 * i);
        }
    }

    // Turn t1, t2 into IoRtcTime

    // put t1, t2 in a struct
}

void tuneRTC(void)
{
    // // Caluclate the offset theta using the formula (in seconds)
    // uint64_t theta = ((IoRtcTimeToSeconds(t1) - IoRtcTimeToSeconds(t0))
    //                 + (IoRtcTimeToSeconds(t2) - IoRtcTimeToSeconds(t3))) / 2;

    // // Get new time for the RTC (offset in s + current time in s) in an IoRtcTime struct
    // IoRtcTime currTime;
    // io_rtc_readTime(&currTime);

    // IoRtcTime newRtcTime = SecondsToIoRtcTime(IoRtcTimeToSeconds(currTime) + theta);

    // // Tune the RTC
    // if (io_rtc_setTime(&newRtcTime) != EXIT_CODE_OK)
    // {
    //     LOG_ERROR("Failed to tune RTC!");
    // }
}

// ============================================================================================================

// bool isLeapYear(uint16_t year)
// {
//     if ((year % 400) == 0) return true;
//     if ((year % 100) == 0) return false;
//     return (year % 4) == 0;
// }

// uint8_t daysInMonth(uint16_t year, uint8_t month)
// {
//     static const uint8_t days_per_month[12] =
//     {
//         31, 28, 31, 30, 31, 30,
//         31, 31, 30, 31, 30, 31
//     };

//     if (month == 2 && isLeapYear(year))
//         return 29;

//     return days_per_month[month - 1];
// }

// uint64_t IoRtcTimeToSeconds(IoRtcTime t)
// {
//     const uint16_t year = 2000 + t.year;
//     uint64_t seconds = 0;

//     //  Add full years since 2000
//     for (uint16_t y = 2000; y < year; ++y)
//     {
//         seconds += isLeapYear(y) ? 366LL : 365LL;
//     }

//     // Add full months of current year
//     for (uint8_t m = 1; m < t.month; ++m)
//     {
//         seconds += daysInMonth(year, m);
//     }

//     // Add full days this month (day is 1-based)
//     seconds += (t.day - 1);
//     // Convert days → seconds
//     seconds *= 86400LL;
//     // Add time-of-day
//     seconds += (uint64_t)t.hours   * 3600LL;
//     seconds += (uint64_t)t.minutes * 60LL;
//     seconds += (uint64_t)t.seconds;
//     return seconds;
// }

// uint8_t calcWeekday(uint16_t year, uint8_t month, uint8_t day)
// {
//     //Sunday = 0
//     if (month < 3)
//     {
//         month += 12;
//         year -= 1;
//     }

//     uint16_t K = year % 100;
//     uint16_t J = year / 100;

//     uint8_t h =
//         (day + (13 * (month + 1)) / 5 +
//          K + K / 4 + J / 4 + 5 * J) % 7;

//     // h = 0 Saturday → convert to Sunday=0
//     return (h + 6) % 7;
// }

// IoRtcTime SecondsToIoRtcTime(uint64_t totalSeconds)
// {
//     IoRtcTime t;
//     uint64_t seconds = totalSeconds;

//     // Extract days since epoch
//     uint64_t days = seconds / 86400LL;
//     seconds %= 86400LL;

//     // Time of day
//     t.hours   = (uint8_t)(seconds / 3600);
//     seconds %= 3600;

//     t.minutes = (uint8_t)(seconds / 60);
//     t.seconds = seconds % 60;

//     // Year
//     uint16_t year = 2000;

//     while (true)
//     {
//         uint64_t daysInYear = isLeapYear(year) ? 366 : 365;
//         if (days >= daysInYear)
//         {
//             days -= daysInYear;
//             ++year;
//         }
//         else
//         {
//             break;
//         }
//     }

//     t.year = (uint8_t)(year - 2000);

//     // Month
//     uint8_t month = 1;

//     while (true)
//     {
//         uint8_t dim = daysInMonth(year, month);

//         if (days >= dim)
//         {
//             days -= dim;
//             ++month;
//         }
//         else
//         {
//             break;
//         }
//     }

//     t.month = month;

//     // Day (1-based)
//     t.day = (uint8_t)(days + 1);
//     // Weekday
//     t.weekdays = calcWeekday(year, month, t.day);
//     return t;
// }
