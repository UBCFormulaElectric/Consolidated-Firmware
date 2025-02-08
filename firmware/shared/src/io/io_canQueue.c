#include "io_canQueue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "message_buffer.h"
#define TX_QUEUE_SIZE 128
#define RX_QUEUE_SIZE 128
#define TX_QUEUE_BYTES sizeof(CanMsg) * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES sizeof(CanMsg) * RX_QUEUE_SIZE

char                        *tx_name;
char                        *rx_name;
MessageBufferHandle_t        tx_queue_id;
MessageBufferHandle_t        rx_queue_id;
StaticQueue_t                tx_queue_control_block;
StaticQueue_t                rx_queue_control_block;
uint8_t                      tx_queue_buf[TX_QUEUE_BYTES];
uint8_t                      rx_queue_buf[RX_QUEUE_BYTES];
static StaticMessageBuffer_t rx_buffer_control_block;
static StaticMessageBuffer_t tx_buffer_control_block;

static bool init_complete = false;

__weak void canTxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canTxQueueOverflowClearCallback() {}
__weak void canRxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canRxQueueOverflowClearCallback() {}

void io_canQueue_init()
{
    // Initialize CAN queues.
    tx_queue_id   = xMessageBufferCreateStatic(TX_QUEUE_SIZE, tx_queue_buf, &tx_buffer_control_block);
    rx_queue_id   = xMessageBufferCreateStatic(RX_QUEUE_SIZE, rx_queue_buf, &rx_buffer_control_block);
    init_complete = true;

    if (tx_queue_id == NULL || rx_queue_id == NULL)
    {
        assert(0);
    }
}

void io_canQueue_pushTx(const CanMsg *tx_msg)
{
    assert(init_complete);
    static uint32_t tx_overflow_count = 0;
    size_t          bytes_sent        = xMessageBufferSend(tx_queue_id, tx_msg, sizeof(CanMsg), 0);

    if (bytes_sent != sizeof(CanMsg))
    {
        canTxQueueOverflowCallBack(++tx_overflow_count);
    }
    else
    {
        canTxQueueOverflowClearCallback();
    }
}

CanMsg io_canQueue_popTx()
{
    assert(init_complete);
    CanMsg msg;
    // Pop a msg of the TX queue
    size_t bytes_received = xMessageBufferReceive(tx_queue_id, &msg, sizeof(CanMsg), portMAX_DELAY);

    assert(bytes_received == sizeof(CanMsg));
    return msg;
}

void io_canQueue_pushRx(const CanMsg *rx_msg)
{
    assert(init_complete);
    static uint32_t rx_overflow_count = 0;

    // We defer reading the CAN RX message to another task by storing the message on the CAN RX queue.
    // use canQueue rx in isr
    size_t bytes_sent = xMessageBufferSend(rx_queue_id, rx_msg, sizeof(CanMsg), 0);
    if (bytes_sent != sizeof(CanMsg))
    {
        canRxQueueOverflowCallBack(++rx_overflow_count);
    }
    else
    {
        canRxQueueOverflowClearCallback();
    }
}

CanMsg io_canQueue_popRx()
{
    assert(init_complete);
    CanMsg msg;
    // Pop a message off the RX queue.
    size_t bytes_received = xMessageBufferReceive(rx_queue_id, &msg, sizeof(CanMsg), portMAX_DELAY);
    assert(bytes_received == sizeof(CanMsg));
    return msg;
}
