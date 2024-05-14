#pragma once

#include <stdint.h>
#include <stdbool.h>
#ifdef TARGET_EMBEDDED
#include "hw_can.h"
#endif

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
    void (*const tx_overflow_clear_callback)();   // Callback on TX queue overflow clear.
    void (*const rx_overflow_clear_callback)();   // Callback on RX queue overflow clear.
} CanConfig;

#define CAN_PAYLOAD_BYTES 8 // TODO: grab from the same place perhaps

#ifdef TARGET_TEST
typedef struct
{
    uint32_t std_id;
    uint32_t dlc; // data length range : [0, 8]
    uint8_t  data[CAN_PAYLOAD_BYTES];
} CanMsg;
#endif

/**
 * Initialize and start the CAN peripheral.
 * @param can_config Config struct.
 */
void io_can_init(const CanConfig *can_config);

/**
 * Enqueue a CAN msg to be transmitted on the bus.
 * Does not block, calls `tx_overflow_callback` if queue is full.
 * @param msg CAN msg to be TXed.
 */
void io_can_pushTxMsgToQueue(const CanMsg *msg);

/**
 * Pops a CAN msg from the TX queue. Blocks until a msg exists in the queue.
 */
void io_can_popTxMsgFromQueue(CanMsg *msg);

/**
 * Transmit a single CAN msg onto the bus.
 */
void io_can_transmitMsgFromQueue(CanMsg *msg);

/**
 * Dequeue a received CAN msg. Blocks until a msg can be dequeued.
 * @param rx_fifo Which RX FIFO to receive a message from.
 */
void io_can_popRxMsgFromQueue(CanMsg *msg);

#ifdef TARGET_EMBEDDED
/**
 * Callback fired by config-specific interrupts to receive a message from a given FIFO.
 * @param msg CAN msg to be populated by RXed msg.
 */
void io_can_msgReceivedCallback(CanMsg *rx_msg);
#endif
