#include "hw_uart.h"

void hw_uart_transmitDma(UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Transmit_DMA(uart->handle, pData, size);
}

void hw_uart_receiveDma(UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Receive_DMA(uart->handle, pData, size);
}

void hw_uart_transmitPoll(UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout)
{
    HAL_UART_Transmit(uart->handle, pData, size, timeout);
}

void hw_uart_receivePoll(UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout)
{
    HAL_UART_Receive(uart->handle, pData, size, timeout);
}

void hw_uart_transmitIt(UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Transmit_IT(uart->handle, pData, size);
}

void hw_uart_receiveIt(UART *uart, uint8_t *pData, uint8_t size)
{
    HAL_UART_Receive_IT(uart->handle, pData, size);
}