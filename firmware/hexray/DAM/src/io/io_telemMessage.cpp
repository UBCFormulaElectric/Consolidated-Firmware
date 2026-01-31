#include "io_telemMessage.hpp"

namespace io::telemMessage
{

Header buildHeader(const uint8_t* payload, uint8_t payload_length)
{
    // TODO: Implement - CRC calculation and header construction
    return {};
}

TelemCanMsg buildCanMsg(const io::CanMsg& rx_msg, float time_offset, uint8_t& size)
{
    // TODO: Implement - Build telemetry CAN message
    return {};
}

} // namespace io::telemMessage
