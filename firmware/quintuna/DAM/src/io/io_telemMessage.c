#include "io_telemMessage.h"
#include "hw_crc.h"

#include <string.h>

typedef enum
{
    TelemMesssageIds_CAN        = 1,
    TelemMesssageIds_NTP        = 2,
    TelemMesssageIds_NTPDate    = 3,
    TelemMessageIds_BaseTimeReg = 4,
} TelemMesssageIds;

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals

#define MAGIC_HIGH 0xAA
#define MAGIC_LOW 0x55
#define HEADER_SIZE 7
#define MAX_FRAME_SIZE (HEADER_SIZE + 100)

TelemMessageHeader io_telemMessage_buildHeader(const uint8_t *payload, const uint8_t payload_length)
{
    TelemMessageHeader header;
    // CRC FUNCTION
    uint32_t crc = hw_crc_calculate((uint32_t *)payload, (uint32_t)(payload_length));
    // https://stackoverflow.com/questions/39646441/how-to-set-stm32-to-generate-standard-crc32
    crc                 = ~crc;
    header.magic[0]     = MAGIC_HIGH;
    header.magic[1]     = MAGIC_LOW;
    header.payload_size = payload_length;
    header.crc          = crc;
    return header;
}

BaseTimeRegMsg io_telemMessage_buildBaseTimeRegMsg(const IoRtcTime *rtc_time)
{
    BaseTimeRegMsg base_time_msg = {
        .msg = { .identifier  = TelemMessageIds_BaseTimeReg,
                 .year        = rtc_time->year,
                 .month       = rtc_time->month,
                 .day         = rtc_time->day,
                 .hour        = rtc_time->hours,
                 .minute      = rtc_time->minutes,
                 .second      = rtc_time->seconds,
                 .microsecond = 0 } // TODO when we switch over to internal RTC
    };
    base_time_msg.header = io_telemMessage_buildHeader((uint8_t *)&base_time_msg.msg, sizeof(base_time_msg.msg));
    return base_time_msg;
}

NtpTimeMsg io_telemMessage_buildNtpTimeMsg(const uint8_t id)
{
    NtpTimeMsg ntp_time_msg = {
        .msg = { .identifier = TelemMesssageIds_NTP, .id = id }
    };
    ntp_time_msg.header = io_telemMessage_buildHeader((uint8_t *)&ntp_time_msg.msg, sizeof(ntp_time_msg.msg));
    return ntp_time_msg;
}

static uint32_t payload_size_from_dlc(const uint32_t dlc)
{
    if (dlc <= 8)
        return dlc;
    if (dlc <= 12)
        return (dlc - 6) * 4;
    if (dlc == 13)
        return 36;
    if (dlc == 14)
        return 48;
    if (dlc == 15)
        return 64;
    assert(dlc <= 15);
    return 0;
}

TelemCanMsg io_telemMessage_buildCanMsg(const CanMsg *rx_msg, const float time_offset, uint8_t *size)
{
    TelemCanMsg telem_msg = { 0 };

    const uint32_t can_payload_size = payload_size_from_dlc(rx_msg->dlc);
    memset(telem_msg.msg.payload, 0, sizeof(telem_msg.msg.payload));
    memcpy(telem_msg.msg.payload, rx_msg->data.data8, can_payload_size);
    telem_msg.msg.identifier  = TelemMesssageIds_CAN;
    telem_msg.msg.can_id      = rx_msg->std_id;
    telem_msg.msg.time_offset = time_offset;

    const uint8_t payload_size = (uint8_t)(sizeof(telem_msg.msg) - sizeof(telem_msg.msg.payload) + can_payload_size);
    telem_msg.header           = io_telemMessage_buildHeader((uint8_t *)&telem_msg.msg, payload_size);

    *size = (uint8_t)(sizeof(telem_msg) - sizeof(telem_msg.msg.payload) + can_payload_size);

    return telem_msg;
}