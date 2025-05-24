#include "io_jsoncan.h"
#include "io_time.h"
#include <string.h>

JsonCanMsg io_jsoncan_copyFromCanMsg(const CanMsg *msg)
{
    JsonCanMsg tx_msg;
    memcpy(tx_msg.data, msg->data.data8, sizeof(tx_msg.data));
    tx_msg.std_id = msg->std_id;
    tx_msg.dlc    = msg->dlc;
    return tx_msg;
}

CanMsg io_jsoncan_copyToCanMsg(const JsonCanMsg *msg)
{
    CanMsg tx_msg;
    memcpy(tx_msg.data.data8, msg->data, sizeof(msg->data));
    tx_msg.std_id    = msg->std_id;
    tx_msg.dlc       = msg->dlc;
    tx_msg.timestamp = io_time_getCurrentMs();
    tx_msg.bus       = false;
    tx_msg.is_fd     = false;
    tx_msg.timestamp = 0;
    return tx_msg;
}
