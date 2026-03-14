#include <array>
#include <cassert>
#include <cstring>
#include <span>

#include "io_telemMessage.hpp"
#include "hw_crc.hpp"

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

} // namespace

namespace io::telemMessage
{

// payload_length is in bytes. CRC hardware consumes 32-bit words, so we zero-pad up to a word boundary.
Header::Header(const uint8_t *payload, uint8_t payload_length)
  : magic{ MAGIC_HIGH, MAGIC_LOW }, payload_size(payload_length), crc(0)
{
    std::array<uint8_t, 256> crc_input{};
    memcpy(crc_input.data(), payload, payload_length);

    const size_t crc_word_count = (static_cast<size_t>(payload_length) + 3U) / 4U;
    const auto   crc_words =
        std::span<const uint32_t>{ reinterpret_cast<const uint32_t *>(crc_input.data()), crc_word_count };

    crc = ~hw::crc::calculate(crc_words);
}

// BaseTimeRegMsg::BaseTimeRegMsg(const IoRtcTime &rtc_time)
// {
//     msg.identifier = static_cast<uint8_t>(TelemMessageIds::BaseTimeReg);
//     msg.year       = rtc_time.year;
//     msg.month      = rtc_time.month;
//     msg.day        = rtc_time.day;
//     msg.hour       = rtc_time.hours;
//     msg.minute     = rtc_time.minutes;
//     msg.second     = rtc_time.seconds;
//     header         = Header(reinterpret_cast<const uint8_t *>(&msg), static_cast<uint8_t>(sizeof(msg)));
// }

TelemCanMsg::TelemCanMsg(const io::CanMsg &rx_msg, float time_offset)
{
    const uint32_t can_payload_size = payloadSizeFromDlc(rx_msg.dlc);

    memset(msg.payload, 0, sizeof(msg.payload));
    memcpy(msg.payload, rx_msg.data.data(), can_payload_size);

    msg.identifier  = static_cast<uint8_t>(TelemMessageIds::CAN);
    msg.can_id      = rx_msg.std_id;
    msg.time_offset = time_offset;

    const uint8_t payload_size = static_cast<uint8_t>(sizeof(msg) - sizeof(msg.payload) + can_payload_size);
    header                     = Header(reinterpret_cast<const uint8_t *>(&msg), payload_size);
}

size_t TelemCanMsg::wireSize() const
{
    return sizeof(Header) + header.payload_size;
}

} // namespace io::telemMessage
