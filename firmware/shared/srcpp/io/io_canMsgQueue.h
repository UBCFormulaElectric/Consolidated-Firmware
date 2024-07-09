#pragma once

#include <cstdint>
#include "hw_can.h"
#include "cmsis_os.h"

/**
 * This module is a CAN driver which manages CAN msg transmission (TX) and reception (RX) via FreeRTOS queues: One for
 * TX, and one for RX.
 *
 * Transmission:
 * 1. Enqueue msgs via `io_can_enqueueTxMsg`. If the TX queue is full, the `tx_overflow_callback` function is called.
 * 2. Pop msgs off the TX queue and transmit them via `io_can_transmitTxMsg`. After popping a msg off the TX queue,
 * `io_can_transmitTxMsg` spins until a CAN TX mailbox becomes available (STM32F412x MCUs have 3) and adds it to a
 * mailbox. After adding to the mailbox, the CAN peripheral hardware will put the msg on the bus.
 *
 * Receiving:
 * 1. When a CAN msg is received, it is passed through a hardware filter (currently configured to accept all msgs with a
 * standard 11-bit identifier) and an interrupt is fired (`HAL_CAN_RxFifo0MsgPendingCallback` or
 * `HAL_CAN_RxFifo1MsgPendingCallback`, depending on the RX hardware FIFO).
 * 2. Both interrupts invoke a callback, which filters by msg ID via an optional `rx_msg_filter` function. If accepted,
 * the RX msg is placed on the RX queue.
 * 3. Pop msgs off the RX queue via `io_can_dequeueRxMsg`, which blocks until a CAN RX msg is successfully dequeued.
 */

// typedef struct
// {
//     bool (*const rx_msg_filter)(uint32_t);        // Optional func to filter RX msgs by ID.
//     void (*const tx_overflow_callback)(uint32_t); // Callback on TX queue overflow.
//     void (*const rx_overflow_callback)(uint32_t); // Callback on RX queue overflow.
//     void (*const tx_overflow_clear_callback)();   // Callback on TX queue overflow clear.
//     void (*const rx_overflow_clear_callback)();   // Callback on RX queue overflow clear.
// } CanConfig;

namespace io
{
// Sizes of CAN TX and RX queues.
#define TX_QUEUE_SIZE 128
#define RX_QUEUE_SIZE 128
#define TX_QUEUE_BYTES (sizeof(hw::CanMsg) * TX_QUEUE_SIZE)
#define RX_QUEUE_BYTES (sizeof(hw::CanMsg) * RX_QUEUE_SIZE)

class CanMsgQueue
{
    osMessageQueueId_t tx_queue_id{};
    osMessageQueueId_t rx_queue_id{};
    StaticQueue_t      tx_queue_control_block{};
    StaticQueue_t      rx_queue_control_block{};
    uint8_t            tx_queue_buf[TX_QUEUE_BYTES]{};
    uint8_t            rx_queue_buf[RX_QUEUE_BYTES]{};

    const osMessageQueueAttr_t tx_queue_attr = {
        .name      = "CAN TX Queue",
        .attr_bits = 0,
        .cb_mem    = &tx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = tx_queue_buf,
        .mq_size   = TX_QUEUE_BYTES,
    };

    const osMessageQueueAttr_t rx_queue_attr = {
        .name      = "CAN RX Queue",
        .attr_bits = 0,
        .cb_mem    = &rx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = rx_queue_buf,
        .mq_size   = RX_QUEUE_BYTES,
    };

    std::function<bool(uint32_t)> rx_msg_filter;
    std::function<void(uint32_t)> tx_overflow_callback;
    std::function<void(uint32_t)> rx_overflow_callback;
    std::function<void()>         tx_overflow_clear_callback;
    std::function<void()>         rx_overflow_clear_callback;

    uint32_t tx_overflow_count = 0;
    uint32_t rx_overflow_count = 0;
    bool     tx_overflow_flag  = false;
    bool     rx_overflow_flag  = false;

  public:
    explicit CanMsgQueue(
        std::function<bool(uint32_t)> rx_msg_filter,
        std::function<void(uint32_t)> tx_overflow_callback,
        std::function<void(uint32_t)> rx_overflow_callback,
        std::function<void()>         tx_overflow_clear_callback,
        std::function<void()>         rx_overflow_clear_callback);

    /**
     * Initialize and start the CAN peripheral.
     * Note: this breaks RAII but embedded moment
     */
    void init();

    /**
     * Enqueue a CAN msg to be transmitted on the bus.
     * Does not block, calls `tx_overflow_callback` if queue is full.
     * @param msg CAN msg to be TXed.
     */
    void pushTxMsgToQueue(const hw::CanMsg *msg);

    /**
     * Pops a CAN msg from the TX queue. Blocks until a msg exists in the queue.
     */
    hw::CanMsg popTxMsgFromQueue();

    /**
     * Dequeue a received CAN msg. Blocks until a msg can be dequeued.
     * @param rx_fifo Which RX FIFO to receive a message from.
     */
    hw::CanMsg popRxMsgFromQueue();

#ifdef TARGET_EMBEDDED
    /**
     * Callback fired by config-specific interrupts to receive a message from a given FIFO.
     * @param msg CAN msg to be populated by RXed msg.
     */
    void pushRxMsgToQueue(hw::CanMsg *rx_msg);
#endif
};
} // namespace io
