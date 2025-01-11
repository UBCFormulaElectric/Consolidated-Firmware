#include "hw_uart.h"

void hw_uart_transmitDma(const UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Transmit_DMA(uart->handle, pData, size);
}

void hw_uart_receiveDma(const UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Receive_DMA(uart->handle, pData, size);
}

void hw_uart_transmitPoll(const UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout)
{
    HAL_UART_Transmit(uart->handle, pData, size, timeout);
}

bool hw_uart_receivePoll(const UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout)
{
    return HAL_UART_Receive(uart->handle, pData, size, timeout) == HAL_OK;
}

void hw_uart_transmitIt(const UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Transmit_IT(uart->handle, pData, size);
}

void hw_uart_receiveIt(const UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Receive_IT(uart->handle, pData, size);
}
