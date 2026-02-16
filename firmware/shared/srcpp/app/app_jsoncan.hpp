#pragma once

#include "io_time.hpp"
#include "io_canMsg.hpp"
#include <cstring>
#include "io_canTx.hpp"


namespace app::jsoncan
{
inline JsonCanMsg copyFromCanMsg(const io::CanMsg &msg)
{
    JsonCanMsg tx_msg(msg.std_id, msg.dlc, {});
    std::memcpy(tx_msg.data.data(), msg.data.data8, sizeof(tx_msg.data));
    return tx_msg;
}

inline io::CanMsg copyToCanMsg(const JsonCanMsg &msg)
{
    io::CanMsg tx_msg;
    std::memcpy(tx_msg.data.data8, msg.data.data(), sizeof(msg.data));
    tx_msg.std_id    = msg.std_id;
    tx_msg.dlc       = msg.dlc;
    tx_msg.timestamp = io::time::getCurrentMs();
    tx_msg.bus       = false;
    tx_msg.is_fd     = false;
    return tx_msg;
}
} // namespace app::jsoncan