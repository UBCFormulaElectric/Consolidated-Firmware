#include "io_telemUart.hpp"

#ifdef TARGET_EMBEDDED
#include "hw_uarts.hpp"

namespace io::telemUart
{

std::expected<void, ErrorCode> transmit(std::span<const uint8_t> tx_data)
{
    return _900k_uart.transmit(tx_data);
}

} // namespace io::telemUart
#endif
