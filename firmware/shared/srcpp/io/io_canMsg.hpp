#pragma once

#include <array>
#include <span>
#include "app_canUtils.hpp"

#include "io_time.hpp"

namespace io
{
#if defined(STM32F412Rx)
inline constexpr size_t CAN_PAYLOAD_BYTES = 8;
#elif defined(STM32H733xx) or defined(STM32H562xx)
inline constexpr size_t CAN_PAYLOAD_BYTES = 64;
#else
#error "Please define what MCU is used."
#endif
struct CanMsg
{
    uint32_t                               std_id;
    uint32_t                               dlc;
    uint32_t                               timestamp;
    std::array<uint8_t, CAN_PAYLOAD_BYTES> data;
    bool                                   is_fd;
    app::can_utils::BusEnum                bus;

    CanMsg(
        const uint32_t                                id,
        const uint32_t                                _dlc,
        const std::array<uint8_t, CAN_PAYLOAD_BYTES> &_data,
        const bool                                    _is_fd,
        const app::can_utils::BusEnum                 _bus)
      : std_id(id), dlc(_dlc), timestamp(time::getCurrentMs()), data(_data), is_fd(_is_fd), bus(_bus)
    {
    }

    CanMsg() = default; // bruh moment

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
