#include "io_crc.hpp"

#include <array>
#include <cstring>

#include "hw_crc.hpp"

namespace io::crc
{

uint32_t calculatePayloadCrc(std::span<const uint8_t> payload)
{
    std::array<uint8_t, 256> crc_input{};
    memcpy(crc_input.data(), payload.data(), payload.size());

    const size_t crc_byte_count = payload.size();
    const auto   crc_bytes =
        std::span<const uint32_t>{ reinterpret_cast<const uint32_t *>(crc_input.data()), crc_byte_count };

    return hw::crc::calculate(crc_bytes);
}

} // namespace io::crc
