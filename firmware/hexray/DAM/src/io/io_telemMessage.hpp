#pragma once

#include <cstdint>
#include <span>
#include <variant>
#include "io_canMsg.hpp"
#include <cstddef>

namespace io::telemMessage
{

enum class TelemMessageIds : uint8_t
{
    CAN = 1,
    NTP = 2,
};

struct [[gnu::packed]] Header
{
    uint8_t  magic[2];
    uint8_t  payload_size;
    uint32_t crc;
    Header() = default;
    Header(const uint8_t *payload, uint8_t payload_length);
};

// Enforce incase packed ever gets ignored/messed up. Sanity
static_assert(sizeof(Header) == 7, "Header must be 7 bytes on the wire");
static_assert(offsetof(Header, magic) == 0, "magic at offset 0");
static_assert(offsetof(Header, payload_size) == 2, "payload_size at offset 2");
static_assert(offsetof(Header, crc) == 3, "crc at offset 3");

struct [[gnu::packed]] CanMsgBody
{
    uint32_t can_id;
    uint64_t time_offset;
    uint8_t  payload[64]; // payload at end so we can truncate
};

struct [[gnu::packed]] TelemMessage
{
    Header          header;
    TelemMessageIds identifier;

    explicit TelemMessage(TelemMessageIds id) : header(), identifier(id) {}

    [[nodiscard]] size_t                   wireSize() const { return sizeof(Header) + header.payload_size; }
    [[nodiscard]] std::span<const uint8_t> asBytes() const
    {
        return { reinterpret_cast<const uint8_t *>(this), wireSize() };
    }
};

struct [[gnu::packed]] TelemCanMsg : TelemMessage
{
    CanMsgBody msg;
    TelemCanMsg() : TelemMessage(TelemMessageIds::CAN) {}
    explicit TelemCanMsg(const io::CanMsg &rx_msg, uint64_t time_offset);
};

struct [[gnu::packed]] NTPMsg : TelemMessage
{
    NTPMsg();
};

using TelemQueueEntry = std::variant<TelemCanMsg, NTPMsg>;

} // namespace io::telemMessage
