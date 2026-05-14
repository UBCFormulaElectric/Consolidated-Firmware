#include <cstring>
#include <span>

#include "io_telemMessage.hpp"
#include "app_crc32.hpp"

namespace
{
constexpr uint8_t MAGIC_HIGH = 0xAA;
constexpr uint8_t MAGIC_LOW  = 0x55;

} // namespace

namespace io::telemMessage
{

// payload_length is in bytes. CRC hardware consumes 32-bit words, so we zero-pad up to a word boundary.
Header::Header(const uint8_t *payload, uint8_t payload_length)
  : magic{ MAGIC_HIGH, MAGIC_LOW }, payload_size(payload_length), crc(0)
{
    uint32_t c = app::crc32::init();
    c          = app::crc32::update(c, payload, payload_length);
    crc        = app::crc32::finalize(c);
}

TelemCanMsg::TelemCanMsg(const io::CanMsg &rx_msg, uint64_t time_offset) : TelemMessage(TelemMessageIds::CAN)
{
    const uint32_t can_payload_size = rx_msg.dlc; // This is actually the payload length

    memset(msg.payload, 0, sizeof(msg.payload));
    memcpy(msg.payload, rx_msg.data.data(), can_payload_size);

    msg.can_id      = rx_msg.std_id;
    msg.time_offset = time_offset;

    const uint8_t payload_size =
        static_cast<uint8_t>(sizeof(identifier) + sizeof(msg) - sizeof(msg.payload) + can_payload_size);
    header = Header(reinterpret_cast<const uint8_t *>(&identifier), payload_size);
}

NTPMsg::NTPMsg() : TelemMessage(TelemMessageIds::NTP)
{
    header = Header(reinterpret_cast<const uint8_t *>(&identifier), static_cast<uint8_t>(sizeof(identifier)));
}

} // namespace io::telemMessage
