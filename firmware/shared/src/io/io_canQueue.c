#include "io_canQueue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void io_canQueue_init(CanQueue *cq)
{
    assert(cq->rx_overflow_callback != NULL);
    assert(cq->rx_overflow_clear_callback != NULL);
    assert(cq->tx_overflow_callback != NULL);
    assert(cq->tx_overflow_clear_callback != NULL);
    // malloc is called here
    sprintf(cq->tx_name, "CAN%d TX Queue", cq->can_number);
    sprintf(cq->rx_name, "CAN%d RX Queue", cq->can_number);
    cq->tx_queue_attr = (osMessageQueueAttr_t){
        .name      = cq->tx_name,
        .attr_bits = 0,
        .cb_mem    = &cq->tx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = cq->tx_queue_buf,
        .mq_size   = TX_QUEUE_BYTES,
    };
    cq->rx_queue_attr = (osMessageQueueAttr_t){
        .name      = cq->rx_name,
        .attr_bits = 0,
        .cb_mem    = &cq->rx_queue_control_block,
        .cb_size   = sizeof(StaticQueue_t),
        .mq_mem    = cq->rx_queue_buf,
        .mq_size   = RX_QUEUE_BYTES,
    };
    // Initialize CAN queues.
    cq->tx_queue_id   = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(CanMsg), &cq->tx_queue_attr);
    cq->rx_queue_id   = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(CanMsg), &cq->rx_queue_attr);
    cq->init_complete = true;
}

void io_canQueue_pushTx(const CanQueue *cq, const CanMsg *tx_msg)
{
    assert(cq->init_complete);
    static uint32_t  tx_overflow_count = 0;
    const osStatus_t s                 = osMessageQueuePut(cq->tx_queue_id, tx_msg, 0, 0);

    if (s != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the tx_msg and invoke the TX overflow callback.
        if (cq->tx_overflow_callback)
            cq->tx_overflow_callback(++tx_overflow_count);
    }
    else
    {
        if (cq->tx_overflow_clear_callback)
            cq->tx_overflow_clear_callback();
    }
}

CanMsg io_canQueue_popTx(const CanQueue *cq)
{
    assert(cq->init_complete);
    CanMsg msg;
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(cq->tx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

void io_canQueue_pushRx(const CanQueue *cq, const CanMsg *rx_msg)
{
    assert(cq->init_complete);
    static uint32_t rx_overflow_count = 0;

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(cq->rx_queue_id, rx_msg, 0, 0) != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        if (cq->rx_overflow_callback != NULL)
            cq->rx_overflow_callback(++rx_overflow_count);
    }
    else
    {
        if (cq->rx_overflow_clear_callback != NULL)
            cq->rx_overflow_clear_callback();
    }
}

CanMsg io_canQueue_popRx(const CanQueue *cq)
{
    assert(cq->init_complete);
    CanMsg msg;
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(cq->rx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}
