#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the USB CDC peripheral, must be called in the `USER CODE BEGIN RTOS_QUEUES` section.
 */
void hw_usb_init();

/**
 * @brief Check if the USB port is connected.
 * @return True if the port is connected, otherwise false.
 */
bool hw_usb_checkConnection();

/**
 * @brief Transmit a buffer over USB of arbritrary length.
 * @param msg A pointer to the message buffer to send.
 * @param len The length of the buffer.
 * @return True if data is transmitted successfully, otherwise false.
 */
bool hw_usb_transmit(uint8_t *msg, uint16_t len);

/**
 * @brief Receive data from the usb host.
 * @param dest A pointer to the destination buffer.
 * @param len Number of bytes to receive.
 * @return True if data is received successfully, otherwise false.
 */
bool hw_usb_receive(uint8_t *dest, uint32_t len);

/**
 * @brief Push a message to the RX queue. To be called from usbd_cdc_if.c, CDC_Receive_FS.
 * @param msg A pointer to the message buffer to push to the queue.
 * @param len Number of bytes to receive.
 * @return True if data is pushed to the queue successfully, otherwise false.
 */
bool hw_usb_pushRxMsgToQueue(uint8_t *msg, uint32_t len);

/**
 * @brief Transmits "hello" repeatedly over USB.
 */
void hw_usb_transmit_example();

/**
 * @brief Logs all received bytes as chars.
 */
void hw_usb_receive_example();
