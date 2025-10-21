#include "io/canQueue.h"

#include <cmsis_os2.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "io/canMsg.h"

static osMessageQueueId_t         rx_queue_id;
static StaticQueue_t              rx_queue_control_block;
uint8_t                           rx_queue_buf[RX_QUEUE_BYTES];
static const osMessageQueueAttr_t rx_queue_attr = { .name      = "CAN RX Queue",
                                                    .attr_bits = 0,
                                                    .cb_mem    = &rx_queue_control_block,
                                                    .cb_size   = sizeof(StaticQueue_t),
                                                    .mq_mem    = rx_queue_buf,
                                                    .mq_size   = RX_QUEUE_BYTES };
static uint32_t                   rx_overflow_count;
static bool                       rx_init_complete;

__weak void canTxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canTxQueueOverflowClearCallback(void) {}
__weak void canRxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canRxQueueOverflowClearCallback(void) {}

void io_canQueue_initRx(void)
{
    // Initialize CAN queues.
    rx_queue_id = osMessageQueueNew(CAN_RX_QUEUE_SIZE, CAN_MSG_SIZE, &rx_queue_attr);
    assert(rx_queue_id != NULL);

    rx_init_complete = true;
}

void io_canQueue_initTx(CanTxQueue *queue)
{
    queue->attr.name      = "CAN TX Queue";
    queue->attr.attr_bits = 0;
    queue->attr.cb_mem    = &queue->control_block;
    queue->attr.cb_size   = sizeof(StaticQueue_t);
    queue->attr.mq_mem    = queue->buffer;
    queue->attr.mq_size   = TX_QUEUE_BYTES;

    queue->id = osMessageQueueNew(CAN_TX_QUEUE_SIZE, CAN_MSG_SIZE, &queue->attr);
    assert(queue->id != NULL);

    queue->init_complete = true;
}

void io_canQueue_pushTx(CanTxQueue *queue, const CanMsg *tx_msg)
{
    assert(queue->init_complete);
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
    assert(queue->init_complete);
    assert(queue->id != NULL);

    CanMsg           msg;
    const osStatus_t s = osMessageQueueGet(queue->id, &msg, NULL, osWaitForever);
    assert(s == osOK);

    return msg;
}

void io_canQueue_pushRx(const CanMsg *rx_msg)
{
    assert(rx_init_complete);

    // We defer reading the CAN RX message to another task by storing the message on the CAN RX queue.
    // use canQueue rx in isr

    osStatus_t status = osMessageQueuePut(rx_queue_id, rx_msg, 0, 0);
    if (status != osOK)
    {
        canRxQueueOverflowCallBack(++rx_overflow_count);
    }
    else
    {
        canRxQueueOverflowClearCallback();
    }
}

CanMsg io_canQueue_popRx(void)
{
    assert(rx_init_complete);

    CanMsg           msg;
    const osStatus_t s = osMessageQueueGet(rx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);

    return msg;
}
