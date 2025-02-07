#pragma once

#include <stdint.h>
#include <stdbool.h>

// Initialize the usb peripheral.
void hw_usb_init();

// Checks if the usb port is connected.
bool hw_usb_checkConnection();

// Transmits usb message of arbritrary length.
// Returns false on failure, otherwise true.
bool hw_usb_transmit(uint8_t *msg, uint16_t len);

// Receive a buffer of bytes over usb.
// Blocks until len bytes are received.
// Dumps into the dest buffer.
// Returns false on failure, otherwise true.
bool hw_usb_receive(uint8_t *dest, uint32_t len);

// Pushes a message onto the internal usb queue,
// for use in CDC_Receive_FS/CDC_Receive_HS.
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len);

// Runs an example loop where the message "hello" is tx-ed repeatedly.
void hw_usb_transmit_example();

// Runs an example loop that logs all received bytes as chars.
void hw_usb_receive_example();
