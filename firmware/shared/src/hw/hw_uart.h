#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

typedef struct
{
    UART_HandleTypeDef *const handle; // pointer to structure containing UART module configuration information
} UART;

/**
 * Transmits an amount of data in DMA mode (non-blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be transmitted.
 */
void hw_uart_transmitDma(const UART *uart, uint8_t *pData, uint8_t size);

/**
 * Receives an amount of data in DMA mode (non-blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be received.
 */
void hw_uart_receiveDma(const UART *uart, uint8_t *pData, uint8_t size);

/**
 * Transmits an amount of data in polling mode (blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be transmitted.
 * @param timeout Timeout duration
 */
void hw_uart_transmitPoll(const UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout);

/**
 * Receives an amount of data in polling mode (blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be received.
 * @param timeout Timeout duration
 */
bool hw_uart_receivePoll(const UART *uart, uint8_t *pData, uint8_t size, uint32_t timeout);

/**
 * Receives an amount of data in interrupt mode (non-blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be received.
 */
void hw_uart_transmitIt(const UART *uart, uint8_t *pData, uint8_t size);

/**
 * Receives an amount of data in interrupt mode (non-blocking).
 * @param uart Pointer to a UART structure.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size Amount of data elements (u8 or u16) to be received.
 */
void hw_uart_receiveIt(const UART *uart, uint8_t *pData, uint8_t size);
