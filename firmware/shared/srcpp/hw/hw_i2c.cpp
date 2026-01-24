#include <cassert>
#include <cstdint>

#include "hw_i2c.hpp"
#include "io_log.h"
#include "cmsis_os2.h"

constexpr uint8_t NUM_DEVICE_READY_TRIALS = 5;

/* --------------------------------------------- I2CBus ------------------------------------------------ */
void hw::i2c::I2CBus::deinit() const
{
    HAL_I2C_DeInit(&handle);
}

void hw::i2c::I2CBus::onTransactionCompleteFromISR() const
{
    assert(taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

/* --------------------------------------------- I2CDevice ------------------------------------------------ */
ExitCode hw::i2c::I2CDevice::waitForNotification() const
{
    const uint32_t notified = ulTaskNotifyTake(pdTRUE, timeoutMs);
    bus.taskInProgress      = nullptr;

    if (notified == 0)
    {
        HAL_I2C_Master_Abort_IT(&bus.handle, static_cast<uint16_t>(targetAddress << 1));
        LOG_WARN("I2C transaction timed out");
        return ExitCode::EXIT_CODE_TIMEOUT;
    }

    return ExitCode::EXIT_CODE_OK;
}

ExitCode hw::i2c::I2CDevice::isTargetReady() const
{
    return hw_utils_convertHalStatus(HAL_I2C_IsDeviceReady(
        &bus.handle, static_cast<uint16_t>(targetAddress << 1), NUM_DEVICE_READY_TRIALS, timeoutMs));
}

ExitCode hw::i2c::I2CDevice::receive(std::span<uint8_t> rx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Master_Receive(
            &bus.handle, static_cast<uint16_t>(targetAddress << 1), rx_buffer.data(),
            static_cast<uint16_t>(rx_buffer.size()), timeoutMs));
    }

    if (bus.taskInProgress != nullptr)
    {
        return ExitCode::EXIT_CODE_BUSY;
    }
    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Master_Receive_IT(
        &bus.handle, static_cast<uint16_t>(targetAddress << 1), rx_buffer.data(),
        static_cast<uint16_t>(rx_buffer.size())));
    if (IS_EXIT_ERR(status))
    {
        bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

ExitCode hw::i2c::I2CDevice::transmit(std::span<const uint8_t> tx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Master_Transmit(
            &bus.handle, static_cast<uint16_t>(targetAddress << 1), const_cast<uint8_t *>(tx_buffer.data()),
            static_cast<uint16_t>(tx_buffer.size()), timeoutMs));
    }

    if (bus.taskInProgress != nullptr)
    {
        return ExitCode::EXIT_CODE_BUSY;
    }
    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Master_Transmit_IT(
        &bus.handle, static_cast<uint16_t>(targetAddress << 1), const_cast<uint8_t *>(tx_buffer.data()),
        static_cast<uint16_t>(tx_buffer.size())));
    if (IS_EXIT_ERR(status))
    {
        bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

ExitCode hw::i2c::I2CDevice::memoryRead(const uint16_t mem_addr, std::span<uint8_t> rx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Read(
            &bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer.data(),
            static_cast<uint16_t>(rx_buffer.size()), timeoutMs));
    }

    if (bus.taskInProgress != nullptr)
    {
        return ExitCode::EXIT_CODE_BUSY;
    }
    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Mem_Read_IT(
        &bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer.data(),
        static_cast<uint16_t>(rx_buffer.size())));
    if (IS_EXIT_ERR(status))
    {
        bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

ExitCode hw::i2c::I2CDevice::memoryWrite(const uint16_t mem_addr, std::span<const uint8_t> tx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Write(
            &bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            const_cast<uint8_t *>(tx_buffer.data()), static_cast<uint16_t>(tx_buffer.size()), timeoutMs));
    }

    if (bus.taskInProgress != nullptr)
    {
        return ExitCode::EXIT_CODE_BUSY;
    }
    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Mem_Write_IT(
        &bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
        const_cast<uint8_t *>(tx_buffer.data()), static_cast<uint16_t>(tx_buffer.size())));
    if (IS_EXIT_ERR(status))
    {
        bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

/* --------------------------------------------- HAL Callbacks ------------------------------------------------ */
extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    const auto &bus = hw::i2c::getBusFromHandle(hi2c);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    const auto &bus = hw::i2c::getBusFromHandle(hi2c);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    const auto &bus = hw::i2c::getBusFromHandle(hi2c);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    const auto &bus = hw::i2c::getBusFromHandle(hi2c);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    LOG_ERROR("I2C error: 0x%X", hi2c->ErrorCode);
}
