#include "hw_spi.h"
#include "hw_gpio.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stm32h7xx_hal_def.h>
#include <stm32h7xx_hal_spi.h>
#include <string.h>

static TaskHandle_t bus_tasks_in_progress[HW_SPI_BUS_COUNT];

static inline bool handletoBus(SPI_HandleTypeDef *const handle, SpiBus *bus)
{
    for (SpiBus i = 0; i < HW_SPI_BUS_COUNT; i++)
    {
        if (spi_bus_handles[i] == handle)
        {
            *bus = (SpiBus)i;
            return true;
        }
    }
    return false;
}

static bool waitForNotification(SpiDevice device)
{
    const SpiDeviceConfig *const config = &spi_device_config[device];

    // Block until a notification is received, or timed out.
    const uint32_t num_notifications     = ulTaskNotifyTake(pdTRUE, config->timeout_ms);
    const bool     transaction_timed_out = num_notifications == 0;

    // Mark this transaction as no longer in progress.
    bus_tasks_in_progress[config->spi_bus] = NULL;

    if (transaction_timed_out)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_SPI_Abort_IT(spi_bus_handles[config->spi_bus]);
    }

    return !transaction_timed_out;
}

static void transactionCompleteHandler(SPI_HandleTypeDef *handle)
{
    SpiBus bus;
    if (!handletoBus(handle, &bus))
    {
        return;
    }

    // Notify the task that started the SPI transaction, if there is a transaction in progress.
    if (bus_tasks_in_progress[bus] != NULL)
    {
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(bus_tasks_in_progress[bus], &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

static void enableNss(SpiDevice device)
{
    const SpiDeviceConfig *const config = &spi_device_config[device];
    hw_gpio_writePin(&config->nss_pin, false);
}

static void disableNss(SpiDevice device)
{
    const SpiDeviceConfig *const config = &spi_device_config[device];
    hw_gpio_writePin(&config->nss_pin, true);
}

bool hw_spi_transmitThenReceive(
    SpiDevice device,
    uint8_t  *tx_buffer,
    uint16_t  tx_buffer_size,
    uint8_t  *rx_buffer,
    uint16_t  rx_buffer_size)

{
    const SpiDeviceConfig *const config = &spi_device_config[device];

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
        const bool status = HAL_SPI_TransmitReceive(
                                spi_bus_handles[config->spi_bus], padded_tx_buffer, padded_rx_buffer, combined_size,
                                config->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    if (bus_tasks_in_progress[config->spi_bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a SPI transaction.
    bus_tasks_in_progress[config->spi_bus] = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_TransmitReceive_IT(
            spi_bus_handles[config->spi_bus], padded_tx_buffer, padded_rx_buffer, combined_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        bus_tasks_in_progress[config->spi_bus] = NULL;
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

bool hw_spi_transmit(SpiDevice device, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    const SpiDeviceConfig *const config = &spi_device_config[device];

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss(device);
        const bool status =
            HAL_SPI_Transmit(spi_bus_handles[config->spi_bus], tx_buffer, tx_buffer_size, config->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    if (bus_tasks_in_progress[config->spi_bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting a SPI transaction.
    bus_tasks_in_progress[config->spi_bus] = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_Transmit_IT(spi_bus_handles[config->spi_bus], tx_buffer, tx_buffer_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        bus_tasks_in_progress[config->spi_bus] = NULL;
        disableNss(device);
        return false;
    }

    const bool status = waitForNotification(device);
    disableNss(device);
    return status;
}

bool hw_spi_receive(SpiDevice device, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    const SpiDeviceConfig *const config = &spi_device_config[device];

    if (bus_tasks_in_progress[config->spi_bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss(device);
        const bool status =
            HAL_SPI_Receive(spi_bus_handles[config->spi_bus], rx_buffer, rx_buffer_size, config->timeout_ms) == HAL_OK;
        disableNss(device);
        return status;
    }

    // Save current task before starting a SPI transaction.
    bus_tasks_in_progress[config->spi_bus] = xTaskGetCurrentTaskHandle();

    enableNss(device);

    if (HAL_SPI_Receive_IT(spi_bus_handles[config->spi_bus], rx_buffer, rx_buffer_size) != HAL_OK)
    {
        // Mark this transaction as no longer in progress.
        bus_tasks_in_progress[config->spi_bus] = NULL;
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
