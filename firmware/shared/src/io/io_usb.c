#include "io_usb.h"
#include <string.h>
#include <assert.h>
#include "cmsis_os.h"
#include "hw_usb.h"

// Sizes of CAN TX and RX queues.
#define TX_QUEUE_SIZE 256
#define RX_QUEUE_SIZE 256
#define TX_QUEUE_BYTES sizeof(pbuff) * TX_QUEUE_SIZE
#define RX_QUEUE_BYTES sizeof(pbuff) * RX_QUEUE_SIZE

// Private globals.
static const USBConfig *config;

static osMessageQueueId_t tx_queue_id;
static osMessageQueueId_t rx_queue_id;
static StaticQueue_t      tx_queue_control_block;
static StaticQueue_t      rx_queue_control_block;
static uint8_t            tx_queue_buf[TX_QUEUE_BYTES];
static uint8_t            rx_queue_buf[RX_QUEUE_BYTES];

static const osMessageQueueAttr_t tx_queue_attr = {
    .name      = "USB TX Queue",
    .attr_bits = 0,
    .cb_mem    = &tx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = tx_queue_buf,
    .mq_size   = TX_QUEUE_BYTES,
};
static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = rx_queue_buf,
    .mq_size   = RX_QUEUE_BYTES,
};

void io_USB_init(const USBConfig *usb)
{
    assert(usb != NULL);
    config = usb;

    // Initialize USB queues.
    tx_queue_id = osMessageQueueNew(TX_QUEUE_SIZE, sizeof(pbuff), &tx_queue_attr);
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(pbuff), &rx_queue_attr);
}

void io_usb_pushTxMsgToQueue(const uint8_t *pbuff)
{
    static uint32_t tx_overflow_count = 0;

    const osStatus_t s = osMessageQueuePut(tx_queue_id, pbuff, 0, 0);
    if (s != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the TX overflow callback.
        if (config->tx_overflow_callback)
            config->tx_overflow_callback(++tx_overflow_count);
    }
    else
    {
        if (config->tx_overflow_clear_callback)
            config->tx_overflow_clear_callback();
    }
}

void io_usb_popTxMsgFromQueue(uint8_t *usb)
{
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(tx_queue_id, pbuff, NULL, osWaitForever);
    assert(s == osOK);
}

void io_usb_transmitMsgFromQueue(uint8_t *pbuff)
{
    // Transmit the tx_msg onto the bus.
    hw_usb_transmit(pbuff);
}

void io_can_popRxMsgFromQueue(uint8_t *pbufff)
{
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(rx_queue_id, pbufff, NULL, osWaitForever);
    assert(s == osOK);
}

void io_can_pushRxMsgToQueue(uint8_t *pbufff)
{
    static uint32_t rx_overflow_count = 0;

    assert(config != NULL);
    if (config->rx_msg_filter != NULL && !config->rx_msg_filter(rx_msg->std_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(rx_queue_id, rx_msg, 0, 0) != osOK)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        if (config->rx_overflow_callback != NULL)
            config->rx_overflow_callback(++rx_overflow_count);
    }
    else
    {
        if (config->rx_overflow_clear_callback != NULL)
            config->rx_overflow_clear_callback();
    }
}

