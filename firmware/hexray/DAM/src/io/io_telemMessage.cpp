#include <cassert>
#include <cstring>
#include <span>

#include "io_telemMessage.hpp"

namespace
{
constexpr uint8_t MAGIC_HIGH = 0xAA;
constexpr uint8_t MAGIC_LOW  = 0x55;

static uint32_t payloadSizeFromDlc(uint32_t dlc)
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
    assert(false);
    return 0;
}

// payload_length is in bytes (same word-count semantics as quintuna hw_crc_calculate)
static io::telemMessage::Header buildHeader(const uint8_t *payload, uint8_t payload_length)
{
    const uint32_t crc = hw::crc::calculate(
        std::span<const uint32_t>{ reinterpret_cast<const uint32_t *>(payload), static_cast<size_t>(payload_length) });

    // https://stackoverflow.com/questions/39646441/how-to-set-stm32-to-generate-standard-crc32
    const uint32_t crc_value = ~crc;

    return io::telemMessage::Header{ .magic        = { MAGIC_HIGH, MAGIC_LOW },
                                     .payload_size = payload_length,
                                     .crc          = crc_value };
}

} // namespace

namespace io::telemMessage
{

BaseTimeRegMsg::BaseTimeRegMsg(const IoRtcTime &rtc_time)
{
    msg.identifier = static_cast<uint8_t>(TelemMessageIds::BaseTimeReg);
    msg.year       = rtc_time.year;
    msg.month      = rtc_time.month;
    msg.day        = rtc_time.day;
    msg.hour       = rtc_time.hours;
    msg.minute     = rtc_time.minutes;
    msg.second     = rtc_time.seconds;
    // removed micros compared to quintuna implementation, our rtc doesn't support
    header = buildHeader(reinterpret_cast<const uint8_t *>(&msg), static_cast<uint8_t>(sizeof(msg)));
}

TelemCanMsg::TelemCanMsg(const io::CanMsg &rx_msg, float time_offset)
{
    const uint32_t can_payload_size = payloadSizeFromDlc(rx_msg.dlc);

    memset(msg.payload, 0, sizeof(msg.payload));
    memcpy(msg.payload, rx_msg.data.data(), can_payload_size);

    msg.identifier  = static_cast<uint8_t>(TelemMessageIds::CAN);
    msg.can_id      = rx_msg.std_id;
    msg.time_offset = time_offset;

    const uint8_t payload_size = static_cast<uint8_t>(sizeof(msg) - sizeof(msg.payload) + can_payload_size);
    header                     = buildHeader(reinterpret_cast<const uint8_t *>(&msg), payload_size);
}

size_t TelemCanMsg::wireSize() const
{
    return sizeof(Header) + header.payload_size;
}

} // namespace io::telemMessage
