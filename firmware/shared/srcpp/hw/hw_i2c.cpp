#include <cassert>
#include "hw_i2c.hpp"
#include "hw_utils.h"
#include "io_log.h"

#define NUM_DEVICE_READY_TRIALS 5

// Static global map for HAL CBs to find correct bus for a handle.
std::unordered_map<I2C_HandleTypeDef *, hw::i2c::I2CBus *> hw::i2c::I2CBus::handle_to_bus_map_;

/* ---------------------- I2CBus ----------------------- */
hw::i2c::I2CBus::I2CBus(I2C_HandleTypeDef *handle) : handle_(handle)
{
    assert(handle != nullptr);
    handle_to_bus_map_[handle_] = this;
}

hw::i2c::I2CBus::~I2CBus()
{
    if (handle_ != nullptr)
    {
        HAL_I2C_DeInit(handle_);
    }
}

hw::i2c::I2CBus *hw::i2c::I2CBus::getBusFromHandle(I2C_HandleTypeDef *handle)
{
    auto it = handle_to_bus_map_.find(handle);
    return (it != handle_to_bus_map_.end()) ? it->second : nullptr;
}

void hw::i2c::I2CBus::onTransactionCompleteFromISR()
{
    if (taskInProgress_ != nullptr)
    {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(taskInProgress_, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}

/* ---------------------- I2CDevice ----------------------- */
hw::i2c::I2CDevice::I2CDevice(I2CBus &bus, uint8_t targetAddr, uint32_t timeoutMs)
  : bus_(bus), targetAddress_(targetAddr), timeoutMs_(timeoutMs)
{
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::waitForNotification()
{
    const uint32_t notified = ulTaskNotifyTake(pdTRUE, timeoutMs_);
    bus_.taskInProgress_    = nullptr;

    if (notified == 0)
    {
        HAL_I2C_Master_Abort_IT(bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1));
        LOG_WARN("I2C transaction timed out");
        return EXIT_CODE_TIMEOUT;
    }
    return EXIT_CODE_OK;
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::isTargetReady() const
{
    return hw_utils_convertHalStatus(HAL_I2C_IsDeviceReady(
        bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), NUM_DEVICE_READY_TRIALS, timeoutMs_));
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::receive(uint8_t *rxBuffer, uint16_t size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Master_Receive(
            bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), rxBuffer, size, timeoutMs_));
    }

    if (bus_.taskInProgress_ != nullptr)
        return EXIT_CODE_BUSY;
    bus_.taskInProgress_ = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(
        HAL_I2C_Master_Receive_IT(bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), rxBuffer, size));
    if (IS_EXIT_ERR(status))
    {
        bus_.taskInProgress_ = nullptr;
        return status;
    }

    return waitForNotification();
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::transmit(const uint8_t *txBuffer, uint16_t size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Master_Transmit(
            bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), (uint8_t *)txBuffer, size, timeoutMs_));
    }

    if (bus_.taskInProgress_ != nullptr)
        return EXIT_CODE_BUSY;
    bus_.taskInProgress_ = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Master_Transmit_IT(
        bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), (uint8_t *)txBuffer, size));
    if (IS_EXIT_ERR(status))
    {
        bus_.taskInProgress_ = nullptr;
        return status;
    }

    return waitForNotification();
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::memoryRead(uint16_t memAddr, uint8_t *rxBuffer, uint16_t size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Read(
            bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), memAddr, I2C_MEMADD_SIZE_8BIT, rxBuffer, size,
            timeoutMs_));
    }

    if (bus_.taskInProgress_ != nullptr)
        return EXIT_CODE_BUSY;
    bus_.taskInProgress_ = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Mem_Read_IT(
        bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), memAddr, I2C_MEMADD_SIZE_8BIT, rxBuffer, size));
    if (IS_EXIT_ERR(status))
    {
        bus_.taskInProgress_ = nullptr;
        return status;
    }

    return waitForNotification();
}

[[nodiscard]] ExitCode hw::i2c::I2CDevice::memoryWrite(uint16_t memAddr, const uint8_t *txBuffer, uint16_t size)
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        return hw_utils_convertHalStatus(HAL_I2C_Mem_Write(
            bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), memAddr, I2C_MEMADD_SIZE_8BIT,
            (uint8_t *)txBuffer, size, timeoutMs_));
    }

    if (bus_.taskInProgress_ != nullptr)
        return EXIT_CODE_BUSY;
    bus_.taskInProgress_ = xTaskGetCurrentTaskHandle();

    const ExitCode status = hw_utils_convertHalStatus(HAL_I2C_Mem_Write_IT(
        bus_.handle_, static_cast<uint16_t>(targetAddress_ << 1), memAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)txBuffer,
        size));
    if (IS_EXIT_ERR(status))
    {
        bus_.taskInProgress_ = nullptr;
        return status;
    }

    return waitForNotification();
}

/* ---------------------- HAL CBs ----------------------- */
extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *handle)
{
    if (auto *bus = hw::i2c::I2CBus::getBusFromHandle(handle))
    {
        bus->onTransactionCompleteFromISR();
    }
}
extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *handle)
{
    if (auto *bus = hw::i2c::I2CBus::getBusFromHandle(handle))
    {
        bus->onTransactionCompleteFromISR();
    }
}
extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *handle)
{
    if (auto *bus = hw::i2c::I2CBus::getBusFromHandle(handle))
    {
        bus->onTransactionCompleteFromISR();
    }
}
extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *handle)
{
    if (auto *bus = hw::i2c::I2CBus::getBusFromHandle(handle))
    {
        bus->onTransactionCompleteFromISR();
    }
}
extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *handle)
{
    LOG_ERROR("I2C error: 0x%X", handle->ErrorCode);
}
