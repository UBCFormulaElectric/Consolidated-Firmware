#pragma once

#include <array>
#include <span>
#include "app_canUtils.hpp"
#include "hw_can.hpp"

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
struct CanMsg : hw::CanMsg
{
    uint32_t                                       timestamp;
    bool                                           is_fd;
    app::can_utils::BusEnum                        bus;

    CanMsg(
        const uint32_t                                id,
        const uint32_t                                _dlc,
        const std::array<uint8_t, CAN_PAYLOAD_BYTES> &_data,
        const bool                                    _is_fd,
        const app::can_utils::BusEnum                 _bus)
      : hw::CanMsg(id, _dlc, _data), timestamp(time::getCurrentMs()), is_fd(_is_fd), bus(_bus)
    {
    }

    CanMsg() = default; // bruh moment
};
} // namespace io
