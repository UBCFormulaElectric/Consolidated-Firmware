#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include <string.h>
#include "io_log.h"
#define RX_QUEUE_SIZE 10


void hw_usb_init();
bool hw_usb_checkConnection();
void hw_usb_transmit(uint8_t *msg, uint16_t len);
uint8_t hw_usb_recieve();
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len);
void hw_usb_transmit_example();
void hw_usb_recieve_example();
