#include "hw_i2c.h"
#include "app_utils.h"
#include "hw_utils.h"
#include "io_log.h"

/* NOTE: Task notifications are used in this driver, since according to FreeRTOS docs they are a faster alternative to
 * binary semaphores.
 * Source:
 * https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/03-Direct-to-task-notifications/02-As-binary-semaphore*/

// Number of attempts made to check if connected device is ready to communicate.
#define NUM_DEVICE_READY_TRIALS 5

static ExitCode waitForNotification(const I2cDevice *device)
{
    // Block until a notification is received, or timed out.
    const uint32_t num_notifications     = ulTaskNotifyTake(pdTRUE, device->timeout_ms);
    const bool     transaction_timed_out = num_notifications == 0;

    // Mark this transaction as no longer in progress.
    device->bus->task_in_progress = NULL;

    if (transaction_timed_out)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_I2C_Master_Abort_IT(device->bus->handle, (uint16_t)(device->target_address << 1));
        LOG_WARN("I2C transaction timed out (did you forget to enable interrupts?)");
    }

    return transaction_timed_out ? EXIT_CODE_TIMEOUT : EXIT_CODE_OK;
}

static void transactionCompleteHandler(I2C_HandleTypeDef *handle)
{
    const I2cBus *const bus = hw_i2c_getBusFromHandle(handle);
    if (bus == NULL)
    {
        return;
    }

    // Notify the task that started the I2C transaction, if there is a transaction in progress.
    if (bus->task_in_progress != NULL)
    {
        BaseType_t higher_priority_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(bus->task_in_progress, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

ExitCode hw_i2c_isTargetReady(const I2cDevice *device)
{
    return hw_utils_convertHalStatus(HAL_I2C_IsDeviceReady(
        device->bus->handle, (uint16_t)(device->target_address << 1), (uint32_t)NUM_DEVICE_READY_TRIALS,
        device->timeout_ms));
}

ExitCode hw_i2c_receive(const I2cDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        return hw_utils_convertHalStatus(HAL_I2C_Master_Receive(
            device->bus->handle, (uint16_t)(device->target_address << 1), rx_buffer, rx_buffer_size,
            device->timeout_ms));
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return EXIT_CODE_BUSY;
    }

    // Save current task before starting an I2C transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    const ExitCode exit = hw_utils_convertHalStatus(HAL_I2C_Master_Receive_IT(
        device->bus->handle, (uint16_t)(device->target_address << 1), rx_buffer, rx_buffer_size));
    if (IS_EXIT_ERR(exit))
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        return exit;
    }

    return waitForNotification(device);
}

ExitCode hw_i2c_transmit(const I2cDevice *device, const uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        return hw_utils_convertHalStatus(HAL_I2C_Master_Transmit(
            device->bus->handle, (uint16_t)(device->target_address << 1), (uint8_t *)tx_buffer, tx_buffer_size,
            device->timeout_ms));
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return EXIT_CODE_BUSY;
    }

    // Save current task before starting an I2C transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    const ExitCode exit = hw_utils_convertHalStatus(HAL_I2C_Master_Transmit_IT(
        device->bus->handle, (uint16_t)(device->target_address << 1), (uint8_t *)tx_buffer, tx_buffer_size));
    if (IS_EXIT_ERR(exit))
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        return exit;
    }

    return waitForNotification(device);
}

ExitCode hw_i2c_memoryRead(const I2cDevice *device, uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Read(
            device->bus->handle, (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer,
            rx_buffer_size, device->timeout_ms));
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return EXIT_CODE_BUSY;
    }

    // Save current task before starting an I2C transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    const ExitCode exit = hw_utils_convertHalStatus(HAL_I2C_Mem_Read_IT(
        device->bus->handle, (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer,
        rx_buffer_size));
    if (IS_EXIT_ERR(exit))
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        return exit;
    }

    return waitForNotification(device);
}

ExitCode
    hw_i2c_memoryWrite(const I2cDevice *device, uint16_t mem_addr, const uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Write(
            device->bus->handle, (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            (uint8_t *)tx_buffer, tx_buffer_size, device->timeout_ms));
    }

    if (device->bus->task_in_progress != NULL)
    {
        // There is a task currently in progress!
        return EXIT_CODE_BUSY;
    }

    // Save current task before starting an I2C transaction.
    device->bus->task_in_progress = xTaskGetCurrentTaskHandle();

    const ExitCode exit = hw_utils_convertHalStatus(
        HAL_I2C_Mem_Write_IT(
            device->bus->handle, (uint16_t)(device->target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            (uint8_t *)tx_buffer, tx_buffer_size) != HAL_OK);
    if (IS_EXIT_ERR(exit))
    {
        // Mark this transaction as no longer in progress.
        device->bus->task_in_progress = NULL;
        return exit;
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

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *handle)
{
    LOG_ERROR("I2C error with code: 0x%X", handle->ErrorCode);
}
