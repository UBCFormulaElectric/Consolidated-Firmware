#pragma once

#include "io_time.hpp"
#include "io_canMsg.hpp"
#include <cstring>

extern "C"
{
#include "io_canRx.h"
}

namespace app::jsoncan
{
inline JsonCanMsg copyFromCanMsg(const io::CanMsg *msg)
{
    JsonCanMsg tx_msg;
    std::memcpy(tx_msg.data, msg->data.data8, sizeof(tx_msg.data));
    tx_msg.std_id = msg->std_id;
    tx_msg.dlc    = msg->dlc;
    return tx_msg;
}

inline io::CanMsg copyToCanMsg(const JsonCanMsg *msg)
{
    io::CanMsg tx_msg;
    std::memcpy(tx_msg.data.data8, msg->data, sizeof(msg->data));
    tx_msg.std_id    = msg->std_id;
    tx_msg.dlc       = msg->dlc;
    tx_msg.timestamp = io::time::getCurrentMs();
    tx_msg.bus       = false;
    tx_msg.is_fd     = false;
    return tx_msg;
}
} // namespace app::jsoncan