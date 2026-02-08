#include <cassert>
#include <cstring>

#include "io_telemMessage.hpp"
namespace
{
constexpr uint8_t MAGIC_HIGH  = 0xAA;
constexpr uint8_t MAGIC_LOW   = 0x55;
constexpr size_t  HEADER_SIZE = 7;

uint32_t payloadSizeFromDlc(uint32_t dlc)
{
    if (dlc <= 8)
    {
        return dlc;
    }
    if (dlc <= 12)
    {
        return (dlc - 6) * 4;
    }
    if (dlc == 13)
    {
        return 32;
    }
    if (dlc == 14)
    {
        return 48;
    }
    if (dlc == 15)
    {
        return 64;
    }
    // Throw error
    assert(false);
    return 0;
}

} // namespace

namespace io::telemMessage
{

Header buildHeader(const uint8_t *payload, uint8_t payload_length)
{
    const auto crc =
        hw_crc_calculate(reinterpret_cast<const uint32_t *>(payload), static_cast<uint32_t>(payload_length));

    // https://stackoverflow.com/questions/39646441/how-to-set-stm32-to-generate-standard-crc32
    const uint32_t crc_value = ~crc;

    return Header{ .magic = { MAGIC_HIGH, MAGIC_LOW }, .payload_size = payload_length, .crc = crc_value };
}

BaseTimeRegMsg buildBaseTimeRegMsg(const IoRtcTime &rtc_time)
{
    BaseTimeRegMsg baseTimeMsg;
    baseTimeMsg.msg.identifier = static_cast<uint8_t>(TelemMessageIds::BaseTimeReg);
    baseTimeMsg.msg.year       = rtc_time.year;
    baseTimeMsg.msg.month      = rtc_time.month;
    baseTimeMsg.msg.day        = rtc_time.day;
    baseTimeMsg.msg.hour       = rtc_time.hours;
    baseTimeMsg.msg.minute     = rtc_time.minutes;
    baseTimeMsg.msg.second     = rtc_time.seconds;
    baseTimeMsg.header =
        buildHeader(reinterpret_cast<const uint8_t *>(&baseTimeMsg.msg), static_cast<uint8_t>(sizeof(baseTimeMsg.msg)));

    return baseTimeMsg;
}

TelemCanMsg buildCanMsg(const io::CanMsg &rx_msg, float time_offset, uint8_t &size)
{
    TelemCanMsg telemCanMsg{};

    const uint32_t can_payload_size = payloadSizeFromDlc(rx_msg.dlc);

    // Copy payload using memcpy (cannot assign arrays directly)
    memset(telemCanMsg.msg.payload, 0, sizeof(telemCanMsg.msg.payload));
    memcpy(telemCanMsg.msg.payload, rx_msg.data.data8, can_payload_size);

    telemCanMsg.msg.identifier  = static_cast<uint8_t>(TelemMessageIds::CAN);
    telemCanMsg.msg.can_id      = rx_msg.std_id;
    telemCanMsg.msg.time_offset = time_offset;

    // Calculate truncated payload size for header
    const uint8_t payload_size =
        static_cast<uint8_t>(sizeof(telemCanMsg.msg) - sizeof(telemCanMsg.msg.payload) + can_payload_size);
    telemCanMsg.header = buildHeader(reinterpret_cast<const uint8_t *>(&telemCanMsg.msg), payload_size);

    // Write output size parameter
    size = static_cast<uint8_t>(sizeof(telemCanMsg) - sizeof(telemCanMsg.msg.payload) + can_payload_size);

    return telemCanMsg;
}

} // namespace io::telemMessage
