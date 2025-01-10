#include "io_canQueue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "cmsis_os.h"
#define TX_QUEUE_SIZE 128
#define RX_QUEUE_SIZE 128
#define TX_QUEUE_BYTES sizeof(CanMsg) * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES sizeof(CanMsg) * RX_QUEUE_SIZE
char*                      tx_name;
char*                      rx_name;
osMessageQueueId_t         tx_queue_id;
osMessageQueueId_t         rx_queue_id;
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
const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "CAN RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = rx_queue_buf,
    .mq_size   = RX_QUEUE_BYTES,
};

static bool init_complete = false;

__weak void canTxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canTxQueueOverflowClearCallback() {}
__weak void canRxQueueOverflowCallBack(const uint32_t overflow_count) {}
__weak void canRxQueueOverflowClearCallback() {}

void io_canQueue_init() {
    // Initialize CAN queues.
    tx_queue_id   = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(CanMsg), &tx_queue_attr);
    rx_queue_id   = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(CanMsg), &rx_queue_attr);
    init_complete = true;
}

void io_canQueue_pushTx(const CanMsg* tx_msg) {
    assert(init_complete);
    static uint32_t  tx_overflow_count = 0;
    const osStatus_t s                 = osMessageQueuePut(tx_queue_id, tx_msg, 0, 0);

    if (s != osOK)
        // If pushing to the queue failed, the queue is full. Discard the tx_msg and invoke the TX overflow callback.
        canTxQueueOverflowCallBack(++tx_overflow_count);
    else
        canTxQueueOverflowClearCallback();
}

CanMsg io_canQueue_popTx() {
    assert(init_complete);
    CanMsg msg;
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(tx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}

void io_canQueue_pushRx(const CanMsg* rx_msg) {
    assert(init_complete);
    static uint32_t rx_overflow_count = 0;

    // We defer reading the CAN RX message to another task by storing the message on the CAN RX queue.
    if (osMessageQueuePut(rx_queue_id, rx_msg, 0, 0) != osOK)
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        canRxQueueOverflowCallBack(++rx_overflow_count);
    else
        canRxQueueOverflowClearCallback();
}

CanMsg io_canQueue_popRx() {
    assert(init_complete);
    CanMsg msg;
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(rx_queue_id, &msg, NULL, osWaitForever);
    assert(s == osOK);
    return msg;
}
