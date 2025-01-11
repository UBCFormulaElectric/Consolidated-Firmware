#include "hw_usb.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "io_log.h"

// setup rx queue
static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_SIZE];
static osMessageQueueId_t rx_queue_id = NULL;
static USBD_HandleTypeDef hUsbDeviceFS;

uint8_t (*usb_transmit_handle)(uint8_t *Buf, uint16_t Len) = NULL;

static const osMessageQueueAttr_t rx_queue_attr = { .name      = "USB RX Queue",
                                                    .attr_bits = 0,
                                                    .cb_mem    = &rx_queue_control_block,
                                                    .cb_size   = sizeof(StaticQueue_t),
                                                    .mq_mem    = rx_queue_buf,
                                                    .mq_size   = sizeof(rx_queue_buf) };

void hw_usb_init(uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len))
{
    usb_transmit_handle = transmit_handle;
    if (rx_queue_id == NULL)
    {
        rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(uint8_t), &rx_queue_attr);
    }
}

bool hw_usb_checkConnection()
{
    return hUsbDeviceFS.dev_state != USBD_STATE_SUSPENDED;
}

void hw_usb_transmit(uint8_t *msg, uint16_t len)
{
    usb_transmit_handle(msg, len);
}

uint8_t hw_usb_recieve()
{
    // receive pops off the byte queue and returns
    uint8_t    res    = 0;
    osStatus_t status = osMessageQueueGet(rx_queue_id, &res, NULL, 100);
    if (status == osOK)
        return res;
    return 0;
}

void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len)
{
    uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    if (len < space)
    {
        LOG_WARN("usb message queue overflow");
    }

    for (uint32_t i = 0; i < len; i += 1)
    {
        // write one byte at a time
        osStatus_t status = osMessageQueuePut(rx_queue_id, &packet[i], 0, 0);
    }
}

void hw_usb_transmit_example(uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len))
{
    // init usb peripheral
    hw_usb_init(transmit_handle);
    osDelay(1000);

    int msg_count = 0;
    for (;;)
    {
        // send hello (without null terminator)
        char     msg[]  = "hello";
        uint8_t *packet = (uint8_t *)msg;
        hw_usb_transmit(packet, 5);

        msg_count += 1;
        LOG_INFO("transmitted \"hello\" %d times", msg_count);

        osDelay(1000);
    }
}

void hw_usb_recieve_example(uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len))
{
    // init usb peripheral
    hw_usb_init(usb_transmit_handle);

    // dump the queue.
    for (int i = 0; true; i += 1)
    {
        uint8_t result = hw_usb_recieve();
        LOG_INFO("char %d: %c", i, (char)result);
        osDelay(1000);
    }
}
