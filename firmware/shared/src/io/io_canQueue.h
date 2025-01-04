#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "io_canMsg.h"

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

typedef struct
{
    bool (*const rx_msg_filter)(uint32_t);        // Optional func to filter RX msgs by ID.
    void (*const tx_overflow_callback)(uint32_t); // Callback on TX queue overflow.
    void (*const rx_overflow_callback)(uint32_t); // Callback on RX queue overflow.
    void (*const tx_overflow_clear_callback)(void);   // Callback on TX queue overflow clear.
    void (*const rx_overflow_clear_callback)(void);   // Callback on RX queue overflow clear.
} CanConfig;

#ifdef TARGET_EMBEDDED
#include "cmsis_os.h"
#define TX_QUEUE_SIZE 128
#define RX_QUEUE_SIZE 128
#define TX_QUEUE_BYTES sizeof(CanMsg) * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES sizeof(CanMsg) * RX_QUEUE_SIZE
#endif
typedef struct {
    void (*tx_overflow_callback)(uint32_t); // Callback on TX queue overflow.
    void (*rx_overflow_callback)(uint32_t); // Callback on RX queue overflow.
    void (*tx_overflow_clear_callback)(void);   // Callback on TX queue overflow clear.
    void (*rx_overflow_clear_callback)(void);    // Callback on RX queue overflow clear.
    uint8_t can_number;
    bool init_complete;

    // populated with init
#ifdef TARGET_EMBEDDED
    char* tx_name;
    char* rx_name;
    osMessageQueueId_t tx_queue_id;
    osMessageQueueId_t rx_queue_id;
    osMessageQueueAttr_t tx_queue_attr;
    osMessageQueueAttr_t rx_queue_attr;
    StaticQueue_t      tx_queue_control_block;
    StaticQueue_t      rx_queue_control_block;
    uint8_t            tx_queue_buf[TX_QUEUE_BYTES];
    uint8_t            rx_queue_buf[RX_QUEUE_BYTES];
#endif
} CanQueue;

/**
 * Initialize and start the CAN peripheral.
 * @param cq CanQueue in question
 */
void io_canQueue_init(CanQueue *cq);

/**
 * Enqueue a CAN msg to be transmitted on the bus.
 * Does not block, calls `tx_overflow_callback` if queue is full.
 * @param cq CanQueue in question
 * @param tx_msg CAN msg to be TXed.
 */
void io_canQueue_pushTx(const CanQueue *cq, const CanMsg *tx_msg);

/**
 * Pops a CAN msg from the TX queue. Blocks until a msg exists in the queue.
 * @param cq CanQueue in question
 */
CanMsg io_canQueue_popTx(const CanQueue *cq);

/**
 * Callback fired by config-specific interrupts to receive a message from a given FIFO.
 * @param cq CanQueue in question
 * @param rx_msg CAN msg to be populated by RXed msg.
 */
void io_canQueue_pushRx(const CanQueue *cq, const CanMsg *rx_msg);

/**
 * Dequeue a received CAN msg. Blocks until a msg can be dequeued.
 */
CanMsg io_canQueue_popRx(const CanQueue* cq);
