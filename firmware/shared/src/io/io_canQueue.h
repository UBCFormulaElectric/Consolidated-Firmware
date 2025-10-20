#pragma once

#include <stdint.h>

#include "io_canMsg.h"

/**
 * This module is a CAN and FD CAN driver which manages CAN msg transmission (TX) and reception (RX) via FreeRTOS
 * queues: One for TX, and one for RX.
 *
 * Depending on the MCU it will provide either the FD CAN or ClASSIC CAN methods
 * ClASSIC and FD CAN messages are both excepted, ClASSIC CAN messages are converted to FD CAN messages
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

#ifdef VCR
#define CAN_TX_QUEUE_SIZE 512
#else
#define CAN_TX_QUEUE_SIZE 128
#endif
#define CAN_RX_QUEUE_SIZE 128

#define CAN_MSG_SIZE sizeof(CanMsg)
#define TX_QUEUE_BYTES (CAN_MSG_SIZE * CAN_TX_QUEUE_SIZE)
#define RX_QUEUE_BYTES (CAN_MSG_SIZE * CAN_RX_QUEUE_SIZE)

#ifdef TARGET_EMBEDDED
#include <cmsis_os.h>
typedef struct
{
    StaticQueue_t        control_block;
    uint8_t              buffer[TX_QUEUE_BYTES];
    osMessageQueueAttr_t attr;
    osMessageQueueId_t   id;
    bool                 init_complete;
} CanTxQueue;
#elif TARGET_TEST
#include "app_utils.h"
EMPTY_STRUCT(CanTxQueue)
#endif

/**
 * Initialize the RX CAN queue.
 */
void io_canQueue_initRx(void);

/**
 * Initialize the TX CAN queue.
 */
void io_canQueue_initTx(CanTxQueue *queue);

/**
 * Enqueue a CAN msg to be transmitted on the bus.
 * Does not block, calls `tx_overflow_callback` if queue is full.
 * @param queue in question
 * @param tx_msg CAN msg to be TXed.
 */
void io_canQueue_pushTx(CanTxQueue *queue, const CanMsg *tx_msg);

/**
 * Pops a CAN msg from the TX queue. Blocks until a msg exists in the queue.
 * @param queue in question
 */
CanMsg io_canQueue_popTx(CanTxQueue *queue);

/**
 * Callback fired by config-specific interrupts to receive a message from a given FIFO.
 * @param rx_msg CAN msg to be populated by RXed msg.
 */
void io_canQueue_pushRx(const CanMsg *rx_msg);

/**
 * Dequeue a received CAN msg. Blocks until a msg can be dequeued.
 */
CanMsg io_canQueue_popRx(void);
