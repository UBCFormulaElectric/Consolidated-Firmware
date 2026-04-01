#pragma once

#include "util_errorCodes.hpp"
#include <cstdint>
#include <span>

namespace hw::usb
{
/**
 * @brief Initialize the USB CDC peripheral, must be called in the `USER CODE BEGIN RTOS_QUEUES` section.
 * @returns whether the USB is ok to be used
 * @note you can call it multiple times
 */
std::expected<void, ErrorCode> init();

/**
 * @brief Check if the USB port is connected (in a "configured" state).
 * @return True if the port is connected, otherwise false.
 */
bool checkConnection();

/**
 * @brief Transmit a buffer over USB of arbritrary length.
 * @param msg A span to the message buffer to send.
 * @return True if data is transmitted successfully, otherwise false.
 */
std::expected<void, ErrorCode> transmit(std::span<uint8_t> msg);

/**
 * @brief Receive data from the usb host.
 * @param dest A span to the destination buffer.
 * @return True if data is received successfully,
 *         false if an error occurred or the timeout has passed.
 * @note that this function will return one byte from the buffer
 * @note NOTE THIS MUST BE PROVIDED EXTERNALLY!!!
 */
void receive(std::span<uint8_t> dest);

// CONNECTION HANDLER

/**
 * Blocks the thread until the USB is connected
 */
void waitForConnected();

/**
 * @return Whether the USB is connected
 * @note This might not be the true source of truth on whether the USB is connected. Please use hw_usb_checkConnection
 * to verify that it is connected.
 * @note Use this in conjunction with hw_usb_waitForConnected to handle async USB behaviour
 */
bool connected();
} // namespace hw::usb
