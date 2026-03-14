#pragma once

#include "io_canMsg.hpp"
#include "app_canUtils.hpp"

namespace app::jsoncan
{
inline JsonCanMsg copyFromCanMsg(const io::CanMsg &msg)
{
    return JsonCanMsg{
        msg.std_id,
        msg.dlc,
        msg.data,
    };
}

inline io::CanMsg copyToCanMsg(const JsonCanMsg &msg)
{
    // TODO lowkey shouldn't exist?
    return io::CanMsg{
        msg.std_id, msg.dlc, msg.data, false, static_cast<can_utils::BusEnum>(0),
    };
}
} // namespace app::jsoncan