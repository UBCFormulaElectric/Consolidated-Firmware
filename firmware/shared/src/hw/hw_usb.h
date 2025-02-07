#pragma once

#include <stdint.h>
#include <stdbool.h>

// initialize the usb peripheral
void hw_usb_init();

// checks if the usb port is connected
bool hw_usb_checkConnection();

// transmits usb message of arbritrary length
// returns false on failure, otherwise true
bool hw_usb_transmit(uint8_t *msg, uint16_t len);

// receive a buffer of bytes over usb
// blocks until len bytes are received
// dumps into the dest buffer
// returns false on failure, otherwise true
bool hw_usb_receive(uint8_t *dest, uint32_t len);

// pushes a message onto the internal usb queue,
// for use in CDC_Receive_FS/CDC_Receive_HS
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len);

// runs an example loop where the message "hello" is tx-ed repeatedly
void hw_usb_transmit_example();

// runs an example loop that logs all received bytes as chars
void hw_usb_receive_example();
