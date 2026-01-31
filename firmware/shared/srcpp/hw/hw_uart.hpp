#pragma once

#include <cstdint>

#ifdef TARGET_EMBEDDED
#include "hw_hal.hpp"
#endif

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

namespace hw
{
class Uart
{
#ifdef TARGET_EMBEDDED
  private:
    UART_HandleTypeDef *const handle; // pointer to structure containing UART module configuration information
  public:
    explicit consteval Uart(UART_HandleTypeDef *in_handle) : handle(in_handle) {}
#endif
  public:
    /**
     * Transmits an amount of data in DMA mode (non-blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be transmitted.
     */
    void transmitDma(uint8_t *pData, uint8_t size) const;

    /**
     * Receives an amount of data in DMA mode (non-blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be received.
     */
    void receiveDma(uint8_t *pData, uint8_t size) const;

    /**
     * Transmits an amount of data in polling mode (blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be transmitted.
     * @param timeout Timeout duration
     */
    void transmitPoll(uint8_t *pData, uint8_t size, uint32_t timeout) const;

    /**
     * Receives an amount of data in polling mode (blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be received.
     * @param timeout Timeout duration
     */
    bool receivePoll(uint8_t *pData, uint8_t size, uint32_t timeout) const;

    /**
     * Receives an amount of data in interrupt mode (non-blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be received.
     */
    void transmitIt(uint8_t *pData, uint8_t size) const;

    /**
     * Receives an amount of data in interrupt mode (non-blocking).
     * @param pData Pointer to data buffer (u8 or u16 data elements).
     * @param size Amount of data elements (u8 or u16) to be received.
     */
    void receiveIt(uint8_t *pData, uint8_t size) const;

    [[nodiscard]] const UART_HandleTypeDef *getHandle() const { return handle; }
};

[[nodiscard]] const Uart &getUartFromHandle(const UART_HandleTypeDef *handle);
} // namespace hw
