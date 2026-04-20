#pragma once

#include <cstdint>
#include <span>
#include "io_canMsg.hpp"

namespace io::telemMessage
{

enum class TelemMessageIds : uint8_t
{
    CAN     = 1,
    NTP     = 2,
    NTPDate = 3,
    // BaseTimeReg = 4, deprecated see quintuna for previous definitions. delete later
};

struct [[gnu::packed]] Header
{
    uint8_t  magic[2];
    uint8_t  payload_size;
    uint32_t crc;
    Header() = default;
    Header(const uint8_t *payload, uint8_t payload_length);
};

struct [[gnu::packed]] CanMsgBody
{
    uint8_t  identifier;
    uint32_t can_id;
    uint64_t time_offset;
    uint8_t  payload[64]; // payload at end so we can truncate
};

struct [[gnu::packed]] TelemMessage
{
    Header                                 header;
    [[nodiscard]] size_t                   wireSize() const { return sizeof(Header) + header.payload_size; }
    [[nodiscard]] std::span<const uint8_t> asBytes() const
    {
        return { reinterpret_cast<const uint8_t *>(this), wireSize() };
    }
};

struct [[gnu::packed]] TelemCanMsg : TelemMessage
{
    CanMsgBody msg;
    TelemCanMsg() = default;
    explicit TelemCanMsg(const io::CanMsg &rx_msg, uint64_t time_offset);
};

struct [[gnu::packed]] NTPMsg : TelemMessage
{
    uint8_t identifier;
    NTPMsg();
};

} // namespace io::telemMessage
