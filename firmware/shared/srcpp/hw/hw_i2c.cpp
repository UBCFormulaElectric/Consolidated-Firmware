#include <cassert>
#include <cstdint>

#include "hw_i2c.hpp"
#include "io_log.hpp"
#include "cmsis_os2.h"

/* --------------------------------------------- d_bus ------------------------------------------------ */
void hw::i2c::bus::onTransactionCompleteFromISR() const
{
    assert(taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

/* --------------------------------------------- device ------------------------------------------------ */
std::expected<void, ErrorCode> hw::i2c::device::waitForNotification() const
{
    const uint32_t notified = ulTaskNotifyTake(pdTRUE, timeoutMs);
    d_bus.taskInProgress    = nullptr;

    if (notified == 0)
    {
        HAL_I2C_Master_Abort_IT(&d_bus.handle, static_cast<uint16_t>(targetAddress << 1));
        LOG_WARN("I2C transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }

    return {};
}

std::expected<void, ErrorCode> hw::i2c::device::receive(std::span<uint8_t> rx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return utils::convertHalStatus(HAL_I2C_Master_Receive(
            &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), rx_buffer.data(),
            static_cast<uint16_t>(rx_buffer.size()), timeoutMs));
    }

    if (d_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    d_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const auto status = utils::convertHalStatus(HAL_I2C_Master_Receive_IT(
        &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), rx_buffer.data(),
        static_cast<uint16_t>(rx_buffer.size())));
    if (not status.has_value())
    {
        d_bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

std::expected<void, ErrorCode> hw::i2c::device::transmit(std::span<const uint8_t> tx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return utils::convertHalStatus(HAL_I2C_Master_Transmit(
            &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), const_cast<uint8_t *>(tx_buffer.data()),
            static_cast<uint16_t>(tx_buffer.size()), timeoutMs));
    }

    if (d_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    d_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const auto status = utils::convertHalStatus(HAL_I2C_Master_Transmit_IT(
        &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), const_cast<uint8_t *>(tx_buffer.data()),
        static_cast<uint16_t>(tx_buffer.size())));
    if (not status.has_value())
    {
        d_bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

std::expected<void, ErrorCode> hw::i2c::device::memoryRead(const uint16_t mem_addr, std::span<uint8_t> rx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return utils::convertHalStatus(HAL_I2C_Mem_Read(
            &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer.data(),
            static_cast<uint16_t>(rx_buffer.size()), timeoutMs));
    }

    if (d_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    d_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const auto status = utils::convertHalStatus(HAL_I2C_Mem_Read_IT(
        &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer.data(),
        static_cast<uint16_t>(rx_buffer.size())));
    if (not status.has_value())
    {
        d_bus.taskInProgress = nullptr;
        return status;
    }

    return waitForNotification();
}

std::expected<void, ErrorCode>
    hw::i2c::device::memoryWrite(const uint16_t mem_addr, std::span<const uint8_t> tx_buffer) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return utils::convertHalStatus(HAL_I2C_Mem_Write(
            &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
            const_cast<uint8_t *>(tx_buffer.data()), static_cast<uint16_t>(tx_buffer.size()), timeoutMs));
    }

    if (d_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    d_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    const auto status = utils::convertHalStatus(HAL_I2C_Mem_Write_IT(
        &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), mem_addr, I2C_MEMADD_SIZE_8BIT,
        const_cast<uint8_t *>(tx_buffer.data()), static_cast<uint16_t>(tx_buffer.size())));
    if (not status.has_value())
    {
        d_bus.taskInProgress = nullptr;
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
