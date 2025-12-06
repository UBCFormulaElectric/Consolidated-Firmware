#include "hw_usb.h"

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>

#include "usbd_cdc_if.h"
#include "io_log.h"

#include <assert.h>

// Select between FS or HS methods.
// F4s (CM4s) only support FS (Full Speed), while H7s (CM7s) support HS (High Speed).
// This macro exposes the USB_DEVICE_HANDLER macro (hUsbDeviceFS or hUsbDeviceHS),
// and the TRANSMIT macro (CDC_Transmit_FS or CDC_Transmit_HS).
// Everything is derived from usbd_cdc_if.h.
#if defined(STM32H562xx)
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
#define RX_QUEUE_SIZE (20)
static StaticQueue_t              rx_queue_control_block;
static uint8_t                    rx_queue_buffer[RX_QUEUE_SIZE];
static const osMessageQueueAttr_t rx_queue_attr = { .name      = "USB RX Queue",
                                                    .attr_bits = 0,
                                                    .cb_mem    = &rx_queue_control_block,
                                                    .cb_size   = sizeof(StaticQueue_t),
                                                    .mq_mem    = rx_queue_buffer,
                                                    .mq_size   = sizeof(rx_queue_buffer) };
static osMessageQueueId_t         rx_queue_id   = NULL;

ExitCode hw_usb_init(void)
{
    if (rx_queue_id == NULL)
    {
        rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(uint8_t), &rx_queue_attr);
        if (rx_queue_id == NULL)
        {
            LOG_ERROR("USB: Failed to create RX queue.");
            return EXIT_CODE_ERROR;
        }
    }
    return EXIT_CODE_OK;
}

bool hw_usb_checkConnection(void)
{
    return USB_DEVICE_HANDLER.dev_state == USBD_STATE_CONFIGURED;
}

ExitCode hw_usb_transmit(uint8_t *msg, const uint16_t len)
{
    const uint8_t status = TRANSMIT(msg, len);
    if (status != USBD_OK)
    {
        LOG_WARN("USB: Transmit handle returned %d status code instead of USBD_OK.", status);
        return EXIT_CODE_ERROR;
    }
    return EXIT_CODE_OK;
}

ExitCode hw_usb_receive(uint8_t *dest, const uint32_t timeout_ms)
{
    if (rx_queue_id == NULL)
    {
        LOG_ERROR("USB: Peripheral not initialized before attempting receive from RX queue.");
        return EXIT_CODE_OK;
    }
    const osStatus_t status = osMessageQueueGet(rx_queue_id, dest, NULL, timeout_ms);
    if (status != osOK)
    {
        if (status == osErrorTimeout)
        {
            LOG_WARN("USB: Timeout occurred while waiting for RX queue.");
            return EXIT_CODE_TIMEOUT;
        }
        LOG_WARN("USB: Queue pop returned non-ok status %d", status);
        return EXIT_CODE_ERROR;
    }
    return EXIT_CODE_OK;
}

bool hw_usb_pushRxMsgToQueue(const uint8_t *msg, const uint32_t len)
{
    if (rx_queue_id == NULL)
    {
        LOG_ERROR("USB: Peripheral not initialized before attempting push to RX queue.");
        return false;
    }

    const uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    if (len > space)
    {
        LOG_ERROR("USB: Receiving message that will overflow RX queue.");
        return false;
    }
    for (uint32_t i = 0; i < len; i += 1)
    {
        // Note: timeout cannot be non-zero in an IRQ.
        const osStatus_t status = osMessageQueuePut(rx_queue_id, &msg[i], 0, 0);
        if (status != osOK)
        {
            LOG_ERROR(
                "USB: Error status encountered when pushing to RX queue, "
                "osMessageQueuePut returned %d instead of osOK.",
                status);
            return false;
        }
    }
    return true;
}

// Connection handling

static bool         usb_connected = false;
static TaskHandle_t usb_task      = NULL;

void hw_usb_connect_callback(void)
{
    usb_connected = true;
    if (usb_task == NULL)
    {
        LOG_WARN("USB: No task to notify.");
        return;
    }
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    // notify the task which is waiting for the USB connection
    vTaskNotifyGiveFromISR(usb_task, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw_usb_disconnect_callback(void)
{
    usb_connected = false;
}

bool hw_usb_connected(void)
{
    return usb_connected;
}

void hw_usb_waitForConnected(void)
{
    if (usb_connected)
        return;

    assert(usb_task == NULL);
    // save which task we need to notify
    usb_task = xTaskGetCurrentTaskHandle();
    // block this task until notification comes
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    UNUSED(num_notifications);
    usb_task = NULL;
}

// EXAMPLES

_Noreturn void hw_usb_transmit_example(void)
{
    // Init usb peripheral.
    int msg_count = 0;
    for (;;)
    {
        // Send hello (without null terminator).
        const char msg[]  = "hello";
        uint8_t   *packet = (uint8_t *)msg;
        ASSERT_EXIT_OK(hw_usb_transmit(packet, 5));

        msg_count += 1;
        LOG_INFO("transmitted \"hello\" %d times", msg_count);

        osDelay(1000);
    }
}

_Noreturn void hw_usb_receive_example(void)
{
    // Dump the queue char by char.
    for (;;)
    {
        uint8_t result = 0;
        if (IS_EXIT_OK(hw_usb_receive(&result, 100)))
            LOG_PRINTF("%c", result);
        osDelay(100);
    }
}
