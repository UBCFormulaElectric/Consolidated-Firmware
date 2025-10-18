#pragma once

#include "app_utils.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the USB CDC peripheral, must be called in the `USER CODE BEGIN RTOS_QUEUES` section.
 * @returns whether the USB is ok to be used
 * @note you can call it multiple times
 */
ExitCode hw_usb_init(void);

/**
 * @brief Check if the USB port is connected (in a "configured" state).
 * @return True if the port is connected, otherwise false.
 */
bool hw_usb_checkConnection(void);

/**
 * @brief Transmit a buffer over USB of arbritrary length.
 * @param msg A pointer to the message buffer to send.
 * @param len The length of the buffer.
 * @return True if data is transmitted successfully, otherwise false.
 */
ExitCode hw_usb_transmit(uint8_t *msg, uint16_t len);

/**
 * @brief Receive data from the usb host.
 * @param dest A pointer to the destination buffer.
 * @param timeout_ms Maximum timeout to wait.
 * @return True if data is received successfully,
 *         false if an error occurred or the timeout has passed.
 * @note that this function will return one byte from the buffer
 */
ExitCode hw_usb_receive(uint8_t *dest, uint32_t timeout_ms);

/**
 * @brief Push a message to the RX queue. To be called from usbd_cdc_if.c, CDC_Receive_FS.
 * @param msg A pointer to the message buffer to push to the queue.
 * @param len Number of bytes to receive.
 * @return True if data is pushed to the queue successfully, otherwise false.
 */
bool hw_usb_pushRxMsgToQueue(const uint8_t *msg, uint32_t len); // TODO make this use ExitCodes

/**
 * @brief Transmits "hello" repeatedly over USB.
 */
_Noreturn void hw_usb_transmit_example(void);

/**
 * @brief Logs all received bytes as chars.
 */
_Noreturn void hw_usb_receive_example(void);

// CONNECTION HANDLER

/**
 * Blocks the thread until the USB is connected
 */
void hw_usb_waitForConnected(void);

/**
 * @return Whether the USB is connected
 * @note This might not be the true source of truth on whether the USB is connected. Please use hw_usb_checkConnection
 * to verify that it is connected.
 * @note Use this in conjunction with hw_usb_waitForConnected to handle async USB behaviour
 */
bool hw_usb_connected(void);

/**
 * @note IF YOU WANT TO USE USB CALLBACKS MAKE SURE TO PUT THIS IN HAL_PCD_ResumeCallback IN USBD_CONF.c (for now)
 */
void hw_usb_connect_callback(void);

/**
 * @note IF YOU WANT TO USE USB CALLBACKS MAKE SURE TO PUT THIS IN HAL_PCD_SuspendCallback IN USBD_CONF.c (for now)
 */
void hw_usb_disconnect_callback(void);
