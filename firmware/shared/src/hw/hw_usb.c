#include "hw_usb.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "io_log.h"

// Select between FS or HS methods.
// F4s (CM4s) only support FS (Full Speed), while H7s (CM7s) support HS (High Speed).
// This macro exposes the USB_DEVICE_HANDLER macro (hUsbDeviceFS or hUsbDeviceHS),
// and the TRANSMIT macro (CDC_Transmit_FS or CDC_Transmit_HS).
// Everything is derived from usbd_cdc_if.h.
#if defined(STM32F412Rx)
#define TRANSMIT(buf, len) (CDC_Transmit_FS(buf, len))
extern USBD_HandleTypeDef hUsbDeviceFS;
#define USB_DEVICE_HANDLER (hUsbDeviceFS)
#elif defined(STM32H733xx)
#define TRANSMIT(buf, len) (CDC_Transmit_HS(buf, len))
extern USBD_HandleTypeDef hUsbDeviceHS;
#define USB_DEVICE_HANDLER (hUsbDeviceHS)
#else
#error Either STM32H733xx or STM32F412Rx must be defined, \
        so that hw_usb knows which handlers to use.
#endif

// Setup the rx queue.
#define RX_QUEUE_SIZE (2048)
static StaticQueue_t              rx_queue_control_block;
static uint8_t                    rx_queue_buffer[RX_QUEUE_SIZE];
static const osMessageQueueAttr_t rx_queue_attr = { .name      = "USB RX Queue",
                                                    .attr_bits = 0,
                                                    .cb_mem    = &rx_queue_control_block,
                                                    .cb_size   = sizeof(StaticQueue_t),
                                                    .mq_mem    = rx_queue_buffer,
                                                    .mq_size   = sizeof(rx_queue_buffer) };
static osMessageQueueId_t         rx_queue_id   = NULL;

void hw_usb_init()
{
    if (rx_queue_id == NULL)
    {
        rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(uint8_t), &rx_queue_attr);
        if (rx_queue_id == NULL)
        {
            LOG_ERROR("USB: Failed to init RX queue.");
        }
        else
        {
            LOG_INFO("USB: Initialized RX queue.");
        }
    }
    else
    {
        LOG_WARN("USB: RX queue already exists when attempting to init USB driver.");
    }
}

bool hw_usb_checkConnection()
{
    return USB_DEVICE_HANDLER.dev_state == USBD_STATE_CONFIGURED;
}

bool hw_usb_transmit(uint8_t *msg, uint16_t len)
{
    uint8_t status = TRANSMIT(msg, len);
    if (status != USBD_OK)
    {
        LOG_WARN("USB: Transmit handle returned %d status code instead of USBD_OK.", status);
        return false;
    }

    return true;
}

bool hw_usb_receive(uint8_t *dest, uint32_t len, uint32_t timeout_ms)
{
    if (rx_queue_id == NULL)
    {
        LOG_ERROR("USB: Peripheral not initialized before attempting receive from RX queue.");
        return false;
    }

    if (len > RX_QUEUE_SIZE)
    {
        LOG_ERROR("USB: Requested to receive %d messages from RX queue, but size of queue is %d.", len, RX_QUEUE_SIZE);

        return false;
    }

    // Wait either until we timeout, or when the queue is full enough.
    uint32_t start_ms   = osKernelGetTickCount();
    uint32_t queued_len = osMessageQueueGetCount(rx_queue_id);
    while (osKernelGetTickCount() - start_ms <= timeout_ms && queued_len < len)
    {
        queued_len = osMessageQueueGetCount(rx_queue_id);
    }

    // Check that we have enough messages in the queue to populate the dest buffer.
    if (queued_len < len)
    {
        return false;
        LOG_WARN("USB: Receive timed out.");
    }

    // Loop through every index in the buffer.
    for (uint32_t i = 0; i < len; i += 1)
    {
        // Dump the byte.
        osStatus_t status = osMessageQueueGet(rx_queue_id, &dest[i], NULL, osWaitForever);

        // Check success.
        if (status != osOK)
        {
            LOG_WARN("USB: Queue pop returned non-ok status %d", status);
            return false;
        }
    }

    return true;
}

bool hw_usb_pushRxMsgToQueue(uint8_t *msg, uint32_t len)
{
    if (rx_queue_id == NULL)
    {
        LOG_ERROR("USB: Peripheral not initialized before attempting push to RX queue.");
        return false;
    }

    uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    if (len > space)
    {
        LOG_ERROR("USB: Receiving message that will overflow RX queue.");
        return false;
    }

    for (uint32_t i = 0; i < len; i += 1)
    {
        // Note: timeout cannot be non-zero in an IRQ.
        osStatus_t status = osMessageQueuePut(rx_queue_id, &msg[i], 0, 0);
        if (status != osOK)
        {
            LOG_ERROR(
                "USB: Error status encountered when pushing to RX queue, "
                "osMessageQueuePut returned %d insteado of osOK.",
                status);
            return false;
        }
    }

    return true;
}

void hw_usb_transmit_example()
{
    // Init usb peripheral.
    int msg_count = 0;
    for (;;)
    {
        // Send hello (without null terminator).
        char     msg[]  = "hello";
        uint8_t *packet = (uint8_t *)msg;
        hw_usb_transmit(packet, 5);

        msg_count += 1;
        LOG_INFO("transmitted \"hello\" %d times", msg_count);

        osDelay(1000);
    }
}

void hw_usb_receive_example()
{
    // Dump the queue char by char.
    for (;;)
    {
        uint8_t result = 0;
        if (hw_usb_receive(&result, 1, 100))
            LOG_PRINTF("%c", result);
        osDelay(100);
    }
}
