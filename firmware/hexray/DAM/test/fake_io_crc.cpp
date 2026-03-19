#include <cstdint>
#include <span>

namespace io::crc
{

uint32_t calculatePayloadCrc(std::span<const uint8_t> payload)
{
    (void)payload;
    return 0U;
}

} // namespace io::crc
