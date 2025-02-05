#include "hw_i2c.h"
#include <assert.h>
#include "main.h"

// Number of attempts made to check if connected device is ready to communicate.
#define NUM_DEVICE_READY_TRIALS 5

static TaskHandle_t bus_tasks_in_progress[HW_I2C_BUS_COUNT];

static inline bool handletoBus(I2C_HandleTypeDef *const handle, I2cBus *bus)
{
    for (I2cBus i = 0; i < HW_I2C_BUS_COUNT; i++)
    {
        if (bus_handles[i] == handle)
        {
            *bus = (I2cBus)i;
            return true;
        }
    }

    return false;
}

static bool waitForNotification(const I2cDevice *device)
{
    // Block until a notification is received.
    const uint32_t num_notfiications = ulTaskNotifyTake(pdTRUE, device->timeout_ms);

    // Mark this transaction as no longer in progress.
    bus_tasks_in_progress[device->bus] = NULL;

    // Success if a notification is received (otherwise the take timed out).
    return num_notfiications > 0;
}

static void transactionCompleteHandler(I2C_HandleTypeDef *handle)
{
    I2cBus bus;
    assert(handletoBus(handle, &bus));

    // Notify the task that started the I2C transaction, if there is a transaction in progress.
    if (bus_tasks_in_progress[bus] != NULL)
    {
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(bus_tasks_in_progress[bus], &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

bool hw_i2c_isTargetReady(const I2cDevice *device)
{
    return HAL_I2C_IsDeviceReady(
               bus_handles[device->bus], (uint16_t)(device->target_address << 1), (uint32_t)NUM_DEVICE_READY_TRIALS,
               device->timeout_ms) == HAL_OK;
}

bool hw_i2c_transmit(const I2cDevice *device, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (bus_tasks_in_progress[device->bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting an I2C transaction.
    bus_tasks_in_progress[device->bus] = xTaskGetCurrentTaskHandle();

    if (HAL_I2C_Master_Transmit_IT(
            bus_handles[device->bus], (uint16_t)(device->target_address << 1), tx_buffer, tx_buffer_size) != HAL_OK)
    {
        return false;
    }

    return waitForNotification(device);
}

bool hw_i2c_receive(const I2cDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (bus_tasks_in_progress[device->bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting an I2C transaction.
    bus_tasks_in_progress[device->bus] = xTaskGetCurrentTaskHandle();

    if (HAL_I2C_Master_Receive_IT(
            bus_handles[device->bus], (uint16_t)(device->target_address << 1), rx_buffer, rx_buffer_size) != HAL_OK)
    {
        return false;
    }

    return waitForNotification(device);
}

bool hw_i2c_memoryWrite(const I2cDevice *device, uint16_t mem_addr, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (bus_tasks_in_progress[device->bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting an I2C transaction.
    bus_tasks_in_progress[device->bus] = xTaskGetCurrentTaskHandle();

    if (HAL_I2C_Mem_Write_IT(
            bus_handles[device->bus], (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            tx_buffer, tx_buffer_size) != HAL_OK)
    {
        return false;
    }

    return waitForNotification(device);
}

bool hw_i2c_memoryRead(const I2cDevice *device, uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (bus_tasks_in_progress[device->bus] != NULL)
    {
        // There is a task currently in progress!
        return false;
    }

    // Save current task before starting an I2C transaction.
    bus_tasks_in_progress[device->bus] = xTaskGetCurrentTaskHandle();

    if (HAL_I2C_Mem_Read_IT(
            bus_handles[device->bus], (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            rx_buffer, rx_buffer_size) != HAL_OK)
    {
        return false;
    }

    return waitForNotification(device);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *handle)
{
    transactionCompleteHandler(handle);
}
