#pragma once

#include <unordered_map>
#include "main.h"
#include "app_utils.h"

namespace hw::i2c
{
class I2CDevice;

class I2CBus
{
  public:
    explicit I2CBus(I2C_HandleTypeDef *handle);
    ~I2CBus(); // deinit I2C peripheral
    static void    registerBus(I2CBus *bus);
    static I2CBus *getBusFromHandle(I2C_HandleTypeDef *handle);
    void           onTransactionCompleteFromISR();

  private:
    friend class I2CDevice;

    I2C_HandleTypeDef                                       *handle_;
    TaskHandle_t                                             taskInProgress_;
    static std::unordered_map<I2C_HandleTypeDef *, I2CBus *> handle_to_bus_map_;
};

class I2CDevice
{
  public:
    explicit I2CDevice(I2CBus &bus, uint8_t targetAddr, uint32_t timeoutMs);

    /**
     * @brief Check if device connected to the given I2C interface is ready for communication.
     * @return EXIT_CODE_OK if connected device is ready to communicate over I2C.
     */
    [[nodiscard]] ExitCode isTargetReady() const;

    /**
     * @brief Receive data from the device connected to the given I2C interface.
     * @param rx_buffer A pointer to the data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @param rx_buffer_size The size of the rx_data buffer.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode receive(uint8_t *rxBuffer, uint16_t size);

    /**
     * @brief Transmit data to the device connected to the given I2C interface.
     * @param tx_buffer A pointer to the data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @param tx_buffer_size The size of the tx_data buffer.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode transmit(const uint8_t *txBuffer, uint16_t size);

    /**
     * @brief Read an amount of data from a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param mem_size The size of the memory address that's going to be read
     * @param rx_buffer A pointer to the data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @param rx_buffer_size The size of the rx_data buffer.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode memoryRead(uint16_t memAddr, uint8_t *rxBuffer, uint16_t size);

    /**
     * @brief Write an amount of data to a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param tx_buffer A pointer to the data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @param tx_buffer_size The size fo the tx_buffer.
     * @return EXIT_CODE_OK if data is read successfully.
     */
    [[nodiscard]] ExitCode memoryWrite(uint16_t memAddr, const uint8_t *txBuffer, uint16_t size);

  private:
    I2CBus  &bus_;
    uint8_t  targetAddress_;
    uint32_t timeoutMs_;

    [[nodiscard]] ExitCode waitForNotification();
};
} // namespace hw::i2c
