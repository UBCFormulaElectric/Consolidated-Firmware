#include "io_canLogging.hpp"

#include <cstring>

#include "app_crc8.hpp"

namespace io::canLogging
{
namespace
{
    struct __attribute__((packed)) Header
    {
        uint8_t  magic;
        uint8_t  dlc;
        uint16_t timestamp;
        uint32_t id;
    };
    static_assert(sizeof(Header) == HEADER_BYTES, "On-disk header must be 8 bytes");
} // namespace

size_t encode(const io::CanMsg &msg, EncodeBuf &out)
{
    const uint8_t dlc = static_cast<uint8_t>(msg.dlc);

    const Header header{
        0xBA,
        dlc,
        static_cast<uint16_t>(msg.timestamp),
        msg.std_id,
    };

    uint8_t crc = app::crc8::init();
    crc         = app::crc8::update(crc, &header, sizeof(header));
    crc         = app::crc8::update(crc, msg.data.data(), dlc);
    crc         = app::crc8::finalize(crc);

    std::memcpy(out.data(), &header, sizeof(header));
    std::memcpy(out.data() + sizeof(header), msg.data.data(), dlc);
    out[sizeof(header) + dlc] = crc;

    return sizeof(header) + dlc + CRC_BYTES;
}
} // namespace io::canLogging
