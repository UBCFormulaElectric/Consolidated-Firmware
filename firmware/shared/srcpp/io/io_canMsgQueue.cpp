#include "io_canMsgQueue.hpp"

#include <cassert>
#include "io_log.hpp"

namespace io
{
CanMsgQueue::CanMsgQueue(
    const char* name,
    void (*const in_tx_overflow_callback)(uint32_t),
    void (*const in_rx_overflow_callback)(uint32_t),
    void (*const in_tx_overflow_clear_callback)(),
    void (*const in_rx_overflow_clear_callback)())
  : tx_overflow_callback(in_tx_overflow_callback),
    rx_overflow_callback(in_rx_overflow_callback),
    tx_overflow_clear_callback(in_tx_overflow_clear_callback),
    rx_overflow_clear_callback(in_rx_overflow_clear_callback),
    rx_queue_attr({
        .name      = name,
        .attr_bits = 0,
        .cb_mem    = &tx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = tx_queue_buf,
        .mq_size   = TX_QUEUE_BYTES,
    }),
    tx_queue_attr({
        .name      = name,
        .attr_bits = 0,
        .cb_mem    = &rx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = rx_queue_buf,
        .mq_size   = RX_QUEUE_BYTES,
    })
{
}

void CanMsgQueue::init()
{
    // Initialize CAN queues.
    tx_queue_id = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(CanMsg), &tx_queue_attr);
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(CanMsg), &rx_queue_attr);
}

void CanMsgQueue::pushTxMsgToQueue(const CanMsg *msg)
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

CanMsg CanMsgQueue::popTxMsgFromQueue() const
{
    CanMsg msg{};
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(tx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

CanMsg CanMsgQueue::popRxMsgFromQueue() const
{
    CanMsg msg{};
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(rx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

void CanMsgQueue::pushRxMsgToQueue(const CanMsg *rx_msg)
{
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
