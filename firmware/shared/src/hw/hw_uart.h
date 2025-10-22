#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "util_errorCodes.h"
#include "hw_hal.h"

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

typedef struct
{
    UART_HandleTypeDef *handle;
    uint32_t            polling_timeout_ms;
    bool                callback_dma;
    void (*transmit_callback)(void);
    void (*receive_callback)(void);
} UartConfig;

typedef struct
{
    const UartConfig config;
    TaskHandle_t     blocked_polling_task;
    bool             rx_callback_pending;
    bool             tx_callback_pending;
} UartDevice;

// Board-specific config: You need to define this function which maps STM32 handle to UartDevice struct!
UartDevice *hw_uart_getDeviceFromHandle(const UART_HandleTypeDef *handle);

/**
 * Transmits an amount of data in blocking mode.
 * @param device UART device.
 * @param data Pointer to data buffer.
 * @param size Number of data bytes.
 */
ExitCode hw_uart_transmit(UartDevice *device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in blocking mode.
 * @param device UART device.
 * @param data Pointer to data buffer.
 * @param size Number of data bytes.
 */
ExitCode hw_uart_receive(UartDevice *device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in inon-blocking mode. Will fire the configured TX callback when complete.
 * @param uart UART device.
 * @param pData Pointer to data buffer.
 * @param Size Amount of data elements to be received.
 */
ExitCode hw_uart_transmitCallback(UartDevice *device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in non-blocking mode. Will fire the configured RX callback when complete.
 * @param uart UART device.
 * @param pData Pointer to data buffer.
 * @param Size Amount of data elements to be received.
 */
ExitCode hw_uart_receiveCallback(UartDevice *device, uint8_t *data, uint16_t size);

ExitCode hw_uart_receive_pooling(UartDevice *device, uint8_t *data, uint16_t size);