#include "io_canQueue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "cmsis_os.h"
#define TX_QUEUE_SIZE 128
#define RX_QUEUE_SIZE 128

#define CAN_MSG_SIZE sizeof(CanMsg)
#define TX_QUEUE_BYTES CAN_MSG_SIZE * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES CAN_MSG_SIZE * RX_QUEUE_SIZE

char                      *tx_name;
char                      *rx_name;
osMessageQueueId_t         tx_queue_id;
MessageBufferHandle_t      rx_queue_id;
StaticQueue_t              tx_queue_control_block;
StaticQueue_t              rx_queue_control_block;
uint8_t                    tx_queue_buf[TX_QUEUE_BYTES];
uint8_t                    rx_queue_buf[RX_QUEUE_BYTES];
const osMessageQueueAttr_t tx_queue_attr = {
    .name      = "CAN TX Queue",
    .attr_bits = 0,
    .cb_mem    = &tx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = tx_queue_buf,
    .mq_size   = TX_QUEUE_BYTES,
};
static StaticMessageBuffer_t rx_buffer_control_block;

static bool init_complete = false;

__weak void canTxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canTxQueueOverflowClearCallback() {}
__weak void canRxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canRxQueueOverflowClearCallback() {}

void io_canQueue_init()
{
    // Initialize CAN queues.
    assert(!init_complete);
    tx_queue_id   = osMessageQueueNew(TX_QUEUE_SIZE, CAN_MSG_SIZE, &tx_queue_attr);
    rx_queue_id   = xMessageBufferCreateStatic(RX_QUEUE_BYTES, rx_queue_buf, &rx_buffer_control_block);
    init_complete = true;

    assert(!(tx_queue_id == NULL || rx_queue_id == NULL));
}

void io_canQueue_pushTx(const CanMsg *tx_msg)
{
    assert(init_complete);
    static uint32_t  tx_overflow_count = 0;
    const osStatus_t s                 = osMessageQueuePut(tx_queue_id, tx_msg, 0, 0);

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

CanMsg io_canQueue_popTx()
{
    assert(init_complete);
    CanMsg msg;
    // Pop a msg of the TX queue
    // size_t bytes_received = xMessageBufferReceive(tx_queue_id, &msg, sizeof(CanMsg), portMAX_DELAY);
    // assert(bytes_received == sizeof(CanMsg));
    const osStatus_t s = osMessageQueueGet(tx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);

    return msg;
}

void io_canQueue_pushRx(const CanMsg *rx_msg)
{
    assert(init_complete);
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
    assert(init_complete);
    CanMsg msg;
    // Pop a message off the RX queue.
    size_t bytes_received = xMessageBufferReceive(rx_queue_id, &msg, CAN_MSG_SIZE, portMAX_DELAY);
    assert(bytes_received == CAN_MSG_SIZE);
    return msg;
}
