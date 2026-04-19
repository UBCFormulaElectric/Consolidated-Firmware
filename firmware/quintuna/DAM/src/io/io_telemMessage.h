#pragma once

#include "../../../../shared/src/io/io_rtc.h"
#include "io_canMsg.h"
#include <stdint.h>

// layer 2

typedef struct __attribute__((packed))
{
    uint8_t  magic[2];
    uint8_t  payload_size;
    uint32_t crc;
} TelemMessageHeader;

TelemMessageHeader io_telemMessage_buildHeader(const uint8_t *payload, uint8_t payload_length);

// layer 3

typedef struct __attribute__((packed))
{
    uint8_t  identifier;
    uint8_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint32_t microsecond;
} BaseTimeRegMsgBody;

typedef struct __attribute__((packed))
{
    TelemMessageHeader header;
    BaseTimeRegMsgBody msg;
} BaseTimeRegMsg;

BaseTimeRegMsg io_telemMessage_buildBaseTimeRegMsg(const IoRtcTime *rtc_time);

typedef struct __attribute__((packed))
{
    uint8_t  identifier;
    uint32_t can_id;
    float    time_offset;
    uint8_t  payload[64]; // note we put the payload at the end so we can cut it off
} TelemCanMsgBody;

typedef struct __attribute__((packed))
{
    TelemMessageHeader header;
    TelemCanMsgBody    msg;
} TelemCanMsg;

TelemCanMsg io_telemMessage_buildCanMsg(const CanMsg *rx_msg, float time_offset, uint8_t *size);