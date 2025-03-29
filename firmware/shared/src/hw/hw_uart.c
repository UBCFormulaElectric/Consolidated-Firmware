#include "hw_uart.h"
#include <assert.h>

static bool waitForNotification(UartDevice *device)
{
    // Block until a notification is received, or timed out.
    const uint32_t num_notifications     = ulTaskNotifyTake(pdTRUE, device->config.polling_timeout_ms);
    const bool     transaction_timed_out = num_notifications == 0;

    // Mark this transaction as no longer in progress.
    device->task_in_progress = NULL;

    if (transaction_timed_out)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_Abort_IT(device->config.handle);
    }

    return !transaction_timed_out;
}

static void transactionCompleteHandler(UART_HandleTypeDef *handle, bool is_tx_transaction)
{
    UartDevice *const device = hw_uart_getDeviceFromHandle(handle);
    if (device == NULL)
    {
        // Unknown device, abort.
        return;
    }

    if (device->is_callback_transaction)
    {
        if (is_tx_transaction)
        {
            assert(device->config.transmit_callback != NULL);
            device->config.transmit_callback();
        }
        else
        {
            assert(device->config.receive_callback != NULL);
            device->config.receive_callback();
        }
    }
    else
    {
        if (device->task_in_progress == NULL)
        {
            // No task in progress, abort.
            return;
        }

        // Notify the task that started the UART transaction, if there is a transaction in progress.
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(device->task_in_progress, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

bool hw_uart_transmit(UartDevice *device, uint8_t *data, uint16_t size)
{
    if (device->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    device->task_in_progress        = xTaskGetCurrentTaskHandle();
    device->is_callback_transaction = false;

    if (HAL_UART_Transmit_IT(device->config.handle, data, size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        device->task_in_progress = NULL;
        return false;
    }

    return waitForNotification(device);
}

bool hw_uart_receive(UartDevice *device, uint8_t *data, uint16_t size)
{
    if (device->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    device->task_in_progress        = xTaskGetCurrentTaskHandle();
    device->is_callback_transaction = false;

    if (HAL_UART_Receive_IT(device->config.handle, data, size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        device->task_in_progress = NULL;
        return false;
    }

    return waitForNotification(device);
}

bool hw_uart_transmitCallback(UartDevice *device, uint8_t *data, uint16_t size)
{
    assert(device->config.transmit_callback != NULL);

    if (device->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    device->task_in_progress        = NULL;
    device->is_callback_transaction = true;

    if (device->config.callback_dma)
    {
        return HAL_UART_Transmit_DMA(device->config.handle, data, size) == HAL_OK;
    }
    else
    {
        return HAL_UART_Transmit_IT(device->config.handle, data, size) == HAL_OK;
    }
}

bool hw_uart_receiveCallback(UartDevice *device, uint8_t *data, uint16_t size)
{
    assert(device->config.receive_callback != NULL);

    if (device->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a UART transaction.
    device->task_in_progress        = NULL;
    device->is_callback_transaction = true;

    if (device->config.callback_dma)
    {
        return HAL_UART_Receive_DMA(device->config.handle, data, size) == HAL_OK;
    }
    else
    {
        return HAL_UART_Receive_IT(device->config.handle, data, size) == HAL_OK;
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
