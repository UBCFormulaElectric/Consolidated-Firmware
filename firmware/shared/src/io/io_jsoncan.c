#include "io_jsoncan.h"
#include <string.h>

JsonCanMsg io_jsoncan_copyFromCanMsg(const CanMsg *msg)
{
    JsonCanMsg tx_msg;
    memcpy(tx_msg.data, msg->data, sizeof(tx_msg.data));
    tx_msg.std_id = msg->std_id;
    tx_msg.dlc    = msg->dlc;
    return tx_msg;
}

CanMsg io_jsoncan_copyToCanMsg(const JsonCanMsg *msg)
{
    CanMsg tx_msg;
    memcpy(tx_msg.data, msg->data, sizeof(tx_msg.data));
    tx_msg.std_id = msg->std_id;
    tx_msg.dlc    = msg->dlc;
    return tx_msg;
}
