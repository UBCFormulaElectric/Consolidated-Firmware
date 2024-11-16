#include "hw_usb.h" 
#include "hw_usbMsgQueue.h"
#include "usbd_cdc_if.h"


#define RX_QUEUE_BYTES 20;
#define MAX_MSG_SIZE 32;
// Private globals.
static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_BYTES];

static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = rx_queue_buf,
    .mq_size   = RX_QUEUE_BYTES,
};

//init, rx, tx 
//using a struct called usb 
//one static instance of the struct pointer to the struct in the init 
//init create queue that returns the id, get rid of usb struct for now 
osMessageQueueId_t hw_USB_CDC_Init(){
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, MAX_MSG_SIZE , &rx_queue_attr);
    return rx_queue_id;
}

uint8_t hw_USB_CDC_Transmit(uint8_t *pbuff){
    CDC_Transmit_FS(&pbuff, len(pbuff));
}

uint8_t hw_USB_CDC_Receive(uint8_t *pbuff){
    CDC_Receive_FS(&pbuff, len(pbuff));
}

void hw_usbQueue_init(const USBD_HandleTypeDef &hUsbDeviceFS)
{
    assert(hUsbDeviceFS != NULL);
    config = hUsbDeviceFS;

    // Initialize USB queues.
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(pbuff), &rx_queue_attr);
}

void hw_usb_pushTxMsgToQueue(const uint8_t *pbuff)
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

void hw_usb_popTxMsgFromQueue(uint8_t *pbuff)
{
    // Pop a msg of the TX queue
    const osStatus_t s = osMessageQueueGet(tx_queue_id, pbuff, NULL, osWaitForever);
    assert(s == osOK);
}

// void hw_usb_transmitMsgFromQueue(uint8_t *pbuff)
// {
//     // Transmit the tx_msg onto the bus.
//     hw_USB_CDC_Transmit(&pbuff);
// }

void hw_usb_popRxMsgFromQueue(uint8_t *pbuff)
{
    // Pop a message off the RX queue.
    const osStatus_t s = osMessageQueueGet(rx_queue_id, pbuff, NULL, osWaitForever);
    assert(s == osOK);
}

void hw_usb_pushRxMsgToQueue(uint8_t *pbuff)
{
    static uint32_t rx_overflow_count = 0;

    assert(config != NULL);
    if (config->rx_msg_filter != NULL && !config->rx_msg_filter(rx_msg->std_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    // We defer reading the RX message to another task by storing the
    // message on the RX queue.
    uint8_t rx_msg = hw_USB_CDC_Receive(uint8_t *pbuff);
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
