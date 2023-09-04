#include "io_can.h"
#include <stdbool.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"

// Sizes of CAN TX and RX queues.
#define TX_QUEUE_SIZE 20
#define RX_QUEUE_SIZE 20
#define TX_QUEUE_BYTES sizeof(CanMsg) * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES sizeof(CanMsg) * RX_QUEUE_SIZE

// Private globals.
static const CanConfig *config;

static osMessageQueueId_t tx_queue_id;
static osMessageQueueId_t rx_queue_id;
static StaticQueue_t      tx_queue_control_block;
static StaticQueue_t      rx_queue_control_block;
static uint8_t            tx_queue_buf[TX_QUEUE_BYTES];
static uint8_t            rx_queue_buf[RX_QUEUE_BYTES];

static const osMessageQueueAttr_t tx_queue_attr = {
    .name      = "CAN TX Queue",
    .attr_bits = 0,
    .cb_mem    = &tx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = tx_queue_buf,
    .mq_size   = TX_QUEUE_BYTES,
};
static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "CAN RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = rx_queue_buf,
    .mq_size   = RX_QUEUE_BYTES,
};

void io_can_init(const CanConfig *can_config)
{
    assert(can_config != NULL);
    assert(can_config->tx_overflow_callback != NULL);
    assert(can_config->tx_overflow_callback != NULL);

    config = can_config;

    // Initialize CAN queues.
    tx_queue_id = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(CanMsg), &tx_queue_attr);
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(CanMsg), &rx_queue_attr);

    // Init config-specific CAN.
    io_can_initConfig(config);
}

void io_can_pushTxMsgToQueue(const CanMsg *msg)
{
    static uint32_t tx_overflow_count = 0;

    if (osMessageQueuePut(tx_queue_id, &msg, 0, 0) != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the TX overflow callback.
        config->tx_overflow_callback(++tx_overflow_count);
    }
}

void io_can_transmitMsgFromQueue(void)
{
    // Pop a msg of the TX queue, then transmit it onto the bus.
    CanMsg tx_msg;
    osMessageQueueGet(tx_queue_id, &tx_msg, NULL, osWaitForever);
    io_can_transmit(&tx_msg);
}

void io_can_popRxMsgFromQueue(CanMsg *msg)
{
    // Pop a message off the RX queue.
    osMessageQueueGet(rx_queue_id, &msg, NULL, osWaitForever);
}

void io_can_msgRceivedCallback(uint32_t rx_fifo)
{
    static uint32_t rx_overflow_count = 0;

    CanMsg rx_msg;
    if (!io_can_receive(rx_fifo, &rx_msg))
    {
        // Early return if RX msg is unavailable.
        return;
    }

    if (config->rx_overflow_callback && !config->rx_msg_filter(rx_msg.msg_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(rx_queue_id, &rx_msg, 0, 0) != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        config->rx_overflow_callback(++rx_overflow_count);
    }
}