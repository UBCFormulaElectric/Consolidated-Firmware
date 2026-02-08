#pragma once

#include <cstdint>
#include "io_canMsg.hpp"

extern "C"
{
#include "hw_crc.h"
#include "io_rtc.h"
}

namespace io::telemMessage
{

enum class TelemMessageIds : uint8_t
{
    CAN         = 1,
    NTP         = 2,
    NTPDate     = 3,
    BaseTimeReg = 4,
};

struct [[gnu::packed]] Header
{
    uint8_t  magic[2];
    uint8_t  payload_size;
    uint32_t crc;
};

struct [[gnu::packed]] BaseTimeRegMsgBody
{
    uint8_t identifier;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct [[gnu::packed]] BaseTimeRegMsg
{
    Header             header;
    BaseTimeRegMsgBody msg;
};

struct [[gnu::packed]] CanMsgBody
{
    uint8_t  identifier;
    uint32_t can_id;
    float    time_offset;
    uint8_t  payload[64]; // payload at end so we can truncate
};

struct [[gnu::packed]] TelemCanMsg
{
    Header     header;
    CanMsgBody msg;
};

Header buildHeader(const uint8_t *payload, uint8_t payload_length);

BaseTimeRegMsg buildBaseTimeRegMsg(const IoRtcTime &rtc_time);

TelemCanMsg buildCanMsg(const io::CanMsg &rx_msg, float time_offset, uint8_t &size);

} // namespace io::telemMessage
