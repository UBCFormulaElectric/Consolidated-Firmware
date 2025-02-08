#include "hw_uart.h"
#include <assert.h>
#include <stdbool.h>

typedef struct
{
    TaskHandle_t task;
    bool         is_callback_transaction;
} UartTaskData;

static UartTaskData uart_tasks_in_progress[HW_UART_DEVICE_COUNT];

static inline bool handleToDevice(UART_HandleTypeDef *const handle, UartDevice *device)
{
    for (UartDevice i = 0; i < HW_UART_DEVICE_COUNT; i++)
    {
        if (uart_device_config[i].uart == handle)
        {
            *device = (UartDevice)i;
            return true;
        }
    }
    return false;
}

static bool waitForNotification(const UartDevice device)
{
    const UartDeviceConfig *const config = &uart_device_config[device];

    // Block until a notification is received, or timed out.
    const uint32_t num_notifications     = ulTaskNotifyTake(pdTRUE, config->polling_timeout_ms);
    const bool     transaction_timed_out = num_notifications == 0;

    // Mark this transaction as no longer in progress.
    uart_tasks_in_progress[device].task = NULL;

    if (transaction_timed_out)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_Abort_IT(config->uart);
    }

    return !transaction_timed_out;
}

static void transactionCompleteHandler(UART_HandleTypeDef *handle, bool is_tx_transaction)
{
    UartDevice device;
    if (!handleToDevice(handle, &device))
    {
        // Unknown device, abort.
        return;
    }

    if (uart_tasks_in_progress[device].task == NULL)
    {
        // No task in progress, abort.
        return;
    }

    if (uart_tasks_in_progress[device].is_callback_transaction)
    {
        const UartDeviceConfig *const config = &uart_device_config[device];

        if (is_tx_transaction)
        {
            assert(config->transmit_callback != NULL);
            config->transmit_callback();
        }
        else
        {
            assert(config->receive_callback != NULL);
            config->receive_callback();
        }
    }
    else
    {
        // Notify the task that started the UART transaction, if there is a transaction in progress.
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(uart_tasks_in_progress[device].task, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

bool hw_uart_transmit(const UartDevice device, uint8_t *data, uint16_t size)
{
    const UartDeviceConfig *const config = &uart_device_config[device];

    if (uart_tasks_in_progress[device].task != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    uart_tasks_in_progress[device].task                    = xTaskGetCurrentTaskHandle();
    uart_tasks_in_progress[device].is_callback_transaction = false;

    if (HAL_UART_Transmit_IT(config->uart, data, size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        uart_tasks_in_progress[device].task = NULL;
        return false;
    }

    return waitForNotification(device);
}

bool hw_uart_receive(const UartDevice device, uint8_t *data, uint16_t size)
{
    const UartDeviceConfig *const config = &uart_device_config[device];

    if (uart_tasks_in_progress[device].task != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    uart_tasks_in_progress[device].task                    = xTaskGetCurrentTaskHandle();
    uart_tasks_in_progress[device].is_callback_transaction = false;

    if (HAL_UART_Receive_IT(config->uart, data, size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        uart_tasks_in_progress[device].task = NULL;
        return false;
    }

    return waitForNotification(device);
}

bool hw_uart_transmitCallback(const UartDevice device, uint8_t *data, uint16_t size)
{
    const UartDeviceConfig *const config = &uart_device_config[device];
    assert(config->transmit_callback != NULL);

    if (uart_tasks_in_progress[device].task != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    uart_tasks_in_progress[device].task                    = xTaskGetCurrentTaskHandle();
    uart_tasks_in_progress[device].is_callback_transaction = true;

    if (config->callback_dma)
    {
        return HAL_UART_Transmit_DMA(config->uart, data, size) == HAL_OK;
    }
    else
    {
        return HAL_UART_Transmit_IT(config->uart, data, size) == HAL_OK;
    }
}

bool hw_uart_receiveCallback(const UartDevice device, uint8_t *data, uint16_t size)
{
    const UartDeviceConfig *const config = &uart_device_config[device];
    assert(config->transmit_callback != NULL);

    if (uart_tasks_in_progress[device].task != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    uart_tasks_in_progress[device].task                    = xTaskGetCurrentTaskHandle();
    uart_tasks_in_progress[device].is_callback_transaction = true;

    if (config->callback_dma)
    {
        return HAL_UART_Receive_DMA(config->uart, data, size) == HAL_OK;
    }
    else
    {
        return HAL_UART_Receive_IT(config->uart, data, size) == HAL_OK;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle, true);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle, false);
}
