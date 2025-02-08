#pragma once

#include <stdint.h>
#include <stdbool.h>

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

// Board-specific config. Must define:
// 1. UartDevice: Enum of all the UART devices.
// 2. uart_device_config: Map of UART device enum to device configs.
#include "hw_uarts.h"

typedef struct
{
    UART_HandleTypeDef *uart;
    uint32_t            polling_timeout_ms;
    bool                callback_dma;
    void (*transmit_callback)(void);
    void (*receive_callback)(void);
} UartDeviceConfig;

extern UartDeviceConfig uart_device_config[HW_UART_DEVICE_COUNT];

/**
 * Transmits an amount of data in blocking mode.
 * @param device UART device.
 * @param data Pointer to data buffer.
 * @param size Number of data bytes.
 */
bool hw_uart_transmit(const UartDevice device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in blocking mode.
 * @param device UART device.
 * @param data Pointer to data buffer.
 * @param size Number of data bytes.
 */
bool hw_uart_receive(const UartDevice device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in inon-blocking mode. Will fire the configured TX callback when complete.
 * @param uart UART device.
 * @param pData Pointer to data buffer.
 * @param Size Amount of data elements to be received.
 */
bool hw_uart_transmitCallback(const UartDevice device, uint8_t *data, uint16_t size);

/**
 * Receives an amount of data in non-blocking mode. Will fire the configured RX callback when complete.
 * @param uart UART device.
 * @param pData Pointer to data buffer.
 * @param Size Amount of data elements to be received.
 */
bool hw_uart_receiveCallback(const UartDevice device, uint8_t *data, uint16_t size);
