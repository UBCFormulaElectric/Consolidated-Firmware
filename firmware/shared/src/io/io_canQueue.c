#include "io_canQueue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "cmsis_os.h"

MessageBufferHandle_t        rx_queue_id;
StaticQueue_t                rx_queue_control_block;
uint8_t                      rx_queue_buf[RX_QUEUE_BYTES];
static StaticMessageBuffer_t rx_buffer_control_block;

__weak void canTxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canTxQueueOverflowClearCallback() {}
__weak void canRxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canRxQueueOverflowClearCallback() {}

void io_canQueue_initRx(void)
{
    // Initialize CAN queues.
    rx_queue_id = xMessageBufferCreateStatic(RX_QUEUE_BYTES, rx_queue_buf, &rx_buffer_control_block);
    assert(rx_queue_id != NULL);
}

void io_canQueue_initTx(CanTxQueue *queue)
{
    queue->attr.name      = "CAN TX Queue";
    queue->attr.attr_bits = 0;
    queue->attr.cb_mem    = &queue->control_block;
    queue->attr.cb_size   = sizeof(StaticQueue_t);
    queue->attr.mq_mem    = queue->buffer;
    queue->attr.mq_size   = TX_QUEUE_BYTES;

    queue->id = osMessageQueueNew(TX_QUEUE_SIZE, CAN_MSG_SIZE, &queue->attr);
    assert(queue->id != NULL);
}

void io_canQueue_pushTx(CanTxQueue *queue, const CanMsg *tx_msg)
{
    assert(queue->id != NULL && tx_msg != NULL);
    const osStatus_t s = osMessageQueuePut(queue->id, tx_msg, 0, 0);

    static uint32_t tx_overflow_count = 0;
    if (s != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the tx_msg and invoke the TX overflow callback.
        canTxQueueOverflowCallBack(++tx_overflow_count);
    }
    else
    {
        canTxQueueOverflowClearCallback();
    }
}

CanMsg io_canQueue_popTx(CanTxQueue *queue)
{
    assert(queue->id != NULL);
    CanMsg           msg;
    const osStatus_t s = osMessageQueueGet(queue->id, &msg, NULL, osWaitForever);
    assert(s == osOK);

    return msg;
}

void io_canQueue_pushRx(const CanMsg *rx_msg)
{
    static uint32_t rx_overflow_count = 0;

    // We defer reading the CAN RX message to another task by storing the message on the CAN RX queue.
    // use canQueue rx in isr
    size_t bytes_sent = xMessageBufferSendFromISR(rx_queue_id, rx_msg, CAN_MSG_SIZE, 0);
    if (bytes_sent != CAN_MSG_SIZE)
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
    CanMsg msg;
    // Pop a message off the RX queue.
    size_t bytes_received = xMessageBufferReceive(rx_queue_id, &msg, CAN_MSG_SIZE, portMAX_DELAY);
    assert(bytes_received == CAN_MSG_SIZE);
    return msg;
}
