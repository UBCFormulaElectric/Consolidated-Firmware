#pragma once

#include <cstdint>
#include <array>
#include "io_canTx.hpp"
#include "io_time.hpp"

#include <span>

#if defined(STM32F412Rx)
#define CAN_PAYLOAD_BYTES 8
#elif defined(STM32H733xx) or defined(STM32H562xx)
#define CAN_PAYLOAD_BYTES 64
#else
#error "Please define what MCU is used."
#endif

namespace io
{
struct CanMsg
{
    uint32_t std_id;
    // classic data length range : [0, 8]
    // fd data length range : [0, 64]
    uint32_t                               dlc;
    uint32_t                               timestamp;
    std::array<uint8_t, CAN_PAYLOAD_BYTES> data;
    io::can_tx::BusEnum                    bus;
    bool                                   is_fd;
    CanMsg(
        const uint32_t                                id,
        const uint32_t                                _dlc,
        const std::array<uint8_t, CAN_PAYLOAD_BYTES> &_data,
        const io::can_tx::BusEnum                     _bus,
        const bool                                    _is_fd)
      : std_id(id), dlc(_dlc), timestamp(time::getCurrentMs()), data(_data), bus(_bus), is_fd(_is_fd)
    {
    }

    // for fuckass C compatilibity
    CanMsg() : std_id(0), dlc(0), timestamp(0), data{}, bus(static_cast<io::can_tx::BusEnum>(0)), is_fd(false) {}

    [[nodiscard]] std::span<const uint16_t, CAN_PAYLOAD_BYTES / 2> getDataAsWords() const
    {
        return std::span<const uint16_t, CAN_PAYLOAD_BYTES / 2>{ reinterpret_cast<const uint16_t *>(data.data()),
                                                                 CAN_PAYLOAD_BYTES / 2 };
    }
    [[nodiscard]] std::span<const uint32_t, CAN_PAYLOAD_BYTES / 4> getDataAsDWords() const
    {
        return std::span<const uint32_t, CAN_PAYLOAD_BYTES / 4>{ reinterpret_cast<const uint32_t *>(data.data()),
                                                                 CAN_PAYLOAD_BYTES / 4 };
    }
};
} // namespace io
