#pragma once

#include <cstdint>
#include "hw_can.hpp"
#include "cmsis_os.h"

/**
 * This module manages CAN msg transmission (TX) and reception (RX) via FreeRTOS queues:
 * One for TX, and one for RX.
 *
 * Transmission:
 * 1. Enqueue msgs via `pushTxMsgToQueue`. If the TX queue is full, the `tx_overflow_callback` function is called.
 * 2. TX Worker pops via `popTxMsgFromQueue` and transmits the msg via the CAN peripheral.
 * After popping a msg off the TX queue, the driver spins until a CAN TX mailbox becomes available
 * (STM32F412x MCUs have 3) and adds it to a mailbox.
 * After adding to the mailbox, the CAN peripheral hardware will put the msg on the bus.
 *
 * Receiving:
 * 1. When a CAN msg is received, it is passed through a hardware filter (currently configured to accept all msgs with a
 * standard 11-bit identifier) and an interrupt is fired (`HAL_CAN_RxFifo0MsgPendingCallback` or
 * `HAL_CAN_RxFifo1MsgPendingCallback`, depending on the RX hardware FIFO).
 * 2. Both interrupts invoke a callback, which filters by msg ID via an optional `rx_msg_filter` function. If accepted,
 * the RX msg is placed on the RX queue.
 * 3. Pop msgs off the RX queue via `io_can_dequeueRxMsg`, which blocks until a CAN RX msg is successfully dequeued.
 */

namespace io
{
// Sizes of CAN TX and RX queues.

template<size_t QUEUE_SIZE = 128u>
class CanMsgQueue
{
    constexpr static size_t QUEUE_SIZE_BYTES = sizeof(CanMsg) * QUEUE_SIZE;
    osMessageQueueId_t queue_id{};
    StaticQueue_t      queue_control_block{};
    uint8_t            queue_buf[QUEUE_SIZE_BYTES]{};

    void (*const overflow_callback)(uint32_t){};
    void (*const overflow_clear_callback)(){};

    uint32_t                   overflow_count = 0;
    bool                       overflow_flag  = false;
    const osMessageQueueAttr_t queue_attr;

  public:
    explicit CanMsgQueue(
        const char *name,
        void (*in_overflow_callback)(uint32_t),
        void (*in_overflow_clear_callback)());

    /**
     * Initialize and start the CAN peripheral.
     * Note: this breaks RAII but embedded moment
     */
    void init();

    /**
     * Enqueue a CAN msg to be transmitted on the bus.
     * Does not block, calls `overflow_callback` if queue is full.
     * @param msg CAN msg to be TXed.
     */
    void pushMsgToQueue(const CanMsg *msg);

    /**
     * Pops a CAN msg from the queue. Blocks until a msg exists in the queue.
     */
    [[nodiscard]] CanMsg popMsgFromQueue() const;
};

template<size_t QUEUE_SIZE>
CanMsgQueue<QUEUE_SIZE>::CanMsgQueue(
    const char* name,
    void (*const in_overflow_callback)(uint32_t),
    void (*const in_overflow_clear_callback)())
  : overflow_callback(in_overflow_callback),
    overflow_clear_callback(in_overflow_clear_callback),
    queue_attr({
        .name      = name,
        .attr_bits = 0,
        .cb_mem    = &this->queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = this->queue_buf,
        .mq_size   = QUEUE_SIZE_BYTES,
    })
{
}

template<size_t QUEUE_SIZE>
void CanMsgQueue<QUEUE_SIZE>::init()
{
    this->queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &this->queue_attr);
}

template<size_t QUEUE_SIZE>
void CanMsgQueue<QUEUE_SIZE>::pushMsgToQueue(const CanMsg *msg)
{
    if (const osStatus_t s = osMessageQueuePut(this->queue_id, msg, 0, 0); s != osOK)
    {
        if (!this->overflow_flag)
        {
            LOG_WARN("%s overflow: %d", this->queue_attr.name, s);
            this->overflow_flag = true;
        }
        this->overflow_callback(++this->overflow_count);
    }
    else
    {
        this->overflow_clear_callback();
        this->overflow_flag = false;
    }
}

template<size_t QUEUE_SIZE>
[[nodiscard]] CanMsg CanMsgQueue<QUEUE_SIZE>::popMsgFromQueue() const
{
    CanMsg msg{};
    const osStatus_t s = osMessageQueueGet(this->queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}
} // namespace io
