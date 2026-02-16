#pragma once

#include "io_time.hpp"
#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include <cstring>

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
    return io::CanMsg{ msg.std_id, msg.dlc, msg.data, static_cast<io::can_tx::BusEnum>(0), false };
}
} // namespace app::jsoncan