#include "io_jsoncan.h"
#include "hw_can.h"
#include <string.h>

void io_jsoncan_pushTxMsgToQueue(const JsonCanMsg *msg)
{
    CanMsg tx_msg;
    memcpy(tx_msg.data, msg->data, sizeof(tx_msg.data));
    io_jsoncan_copyToCanMsg(msg, &tx_msg);
    io_can_pushTxMsgToQueue(&tx_msg);
}

void io_jsoncan_copyToCanMsg(const JsonCanMsg *src, CanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}

void io_jsoncan_copyFromCanMsg(const CanMsg *src, JsonCanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}
