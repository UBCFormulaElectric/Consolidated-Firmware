#include "hw/uart.hpp"

namespace hw
{
void Uart::transmitDma(uint8_t *pData, uint8_t size) const
{
    HAL_UART_Transmit_DMA(handle, pData, size);
}

void Uart::receiveDma(uint8_t *pData, uint8_t size) const
{
    HAL_UART_Receive_DMA(handle, pData, size);
}

void Uart::transmitPoll(uint8_t *pData, uint8_t size, uint32_t timeout) const
{
    HAL_UART_Transmit(handle, pData, size, timeout);
}

bool Uart::receivePoll(uint8_t *pData, uint8_t size, uint32_t timeout) const
{
    return HAL_UART_Receive(handle, pData, size, timeout) == HAL_OK;
}

void Uart::transmitIt(uint8_t *pData, uint8_t size) const
{
    HAL_UART_Transmit_IT(handle, pData, size);
}

void Uart::receiveIt(uint8_t *pData, uint8_t size) const
{
    HAL_UART_Receive_IT(handle, pData, size);
}
} // namespace hw
