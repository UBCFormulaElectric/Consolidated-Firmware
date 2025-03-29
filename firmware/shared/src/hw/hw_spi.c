#include "hw_spi.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool waitForNotification(const SpiDevice *device)
{
    // Block until a notification is received, or timed out.
    const uint32_t num_notifications     = ulTaskNotifyTake(pdTRUE, device->timeout_ms);
    const bool     transaction_timed_out = num_notifications == 0;

    // Mark this transaction as no longer in progress.
    device->bus->task_in_progress = NULL;

    if (transaction_timed_out)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_SPI_Abort_IT(device->bus->handle);
    }

    return !transaction_timed_out;
}

static void transactionCompleteHandler(SPI_HandleTypeDef *handle)
{
    const SpiBus *const bus = hw_spi_getBusFromHandle(handle);
    if (bus == NULL)
    {
        return;
    }

    // Notify the task that started the SPI transaction, if there is a transaction in progress.
    if (bus->task_in_progress != NULL)
    {
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(bus->task_in_progress, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

static void enableNss(const SpiDevice *device)
{
    hw_gpio_writePin(device->nss_pin, false);
}

static void disableNss(const SpiDevice *device)
{
    hw_gpio_writePin(device->nss_pin, true);
}

bool hw_spi_transmitThenReceive(
    const SpiDevice *device,
    uint8_t         *tx_buffer,
    uint16_t         tx_buffer_size,
    uint8_t         *rx_buffer,
    uint16_t         rx_buffer_size)

{
    // HAL_SPI_TransmitReceive_IT requires tx buffer and rx buffer to be of size equal to number of bytes to transmit
    // and receive
    const uint16_t combined_size = tx_buffer_size + rx_buffer_size;
    uint8_t        padded_tx_buffer[combined_size];
    uint8_t        padded_rx_buffer[combined_size];

    // Copy tx_buffer into beginning of larger padded_tx_buffer
    memcpy(padded_tx_buffer, tx_buffer, tx_buffer_size);

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss(device);
        const bool status =
            HAL_SPI_TransmitReceive(
                device->bus->handle, padded_tx_buffer, padded_rx_buffer, combined_size, device->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a SPI transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_TransmitReceive_IT(device->bus->handle, padded_tx_buffer, padded_rx_buffer, combined_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        disableNss(device);
        return false;
    }

    const bool status = waitForNotification(device);
    disableNss(device);

    /* Data will not be returned over SPI until command has finished, so data in first tx_buffer_size bytes not relevant
    Copy entries at the end of padded_rx_buffer back into rx_buffer */
    memcpy(rx_buffer, &padded_rx_buffer[tx_buffer_size], rx_buffer_size);

    return status;
}

bool hw_spi_transmit(const SpiDevice *device, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss(device);
        const bool status =
            HAL_SPI_Transmit(device->bus->handle, tx_buffer, tx_buffer_size, device->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a SPI transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_Transmit_IT(device->bus->handle, tx_buffer, tx_buffer_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        disableNss(device);
        return false;
    }

    const bool status = waitForNotification(device);
    disableNss(device);
    return status;
}

bool hw_spi_receive(const SpiDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss(device);
        const bool status =
            HAL_SPI_Receive(device->bus->handle, rx_buffer, rx_buffer_size, device->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    // Save current task before starting a SPI transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_Receive_IT(device->bus->handle, rx_buffer, rx_buffer_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        disableNss(device);
        return false;
    }

    const bool status = waitForNotification(device);
    disableNss(device);
    return status;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}
