#include "io_canMsgQueue.h"
#include "io_log.h"

#include <cstring>
#include <cassert>

namespace io
{
void CanMsgQueue::init()
{
    // Initialize CAN queues.
    tx_queue_id = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(hw::can::CanMsg), &tx_queue_attr);
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(hw::can::CanMsg), &rx_queue_attr);
}

void CanMsgQueue::pushTxMsgToQueue(const hw::can::CanMsg *msg)
{
    if (const osStatus_t s = osMessageQueuePut(tx_queue_id, msg, 0, 0); s != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the TX overflow callback.
        if (!tx_overflow_flag)
        {
            LOG_WARN("TX queue overflow: %d", s);
            tx_overflow_flag = true;
        }
        tx_overflow_callback(++tx_overflow_count);
    }
    else
    {
        tx_overflow_clear_callback();
        tx_overflow_flag = false;
    }
}

hw::can::CanMsg CanMsgQueue::popTxMsgFromQueue()
{
    hw::can::CanMsg msg{};
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(tx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

hw::can::CanMsg CanMsgQueue::popRxMsgFromQueue()
{
    hw::can::CanMsg msg{};
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(rx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

void CanMsgQueue::pushRxMsgToQueue(const hw::can::CanMsg *rx_msg)
{
    if (!rx_msg_filter(rx_msg->std_id))
        // Early return if we don't care about this msg via configured filter func.
        return;

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(rx_queue_id, rx_msg, 0, 0) != osOK)
    {
        if (!rx_overflow_flag)
        {
            LOG_WARN("RX queue overflow");
            rx_overflow_flag = true;
        }
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        rx_overflow_callback(++rx_overflow_count);
    }
    else
    {
        rx_overflow_clear_callback();
        rx_overflow_flag = false;
    }
}
} // namespace io
