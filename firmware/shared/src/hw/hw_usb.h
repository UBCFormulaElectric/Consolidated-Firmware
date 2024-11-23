#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include <string.h>

osMessageQueueId_t hw_usb_init();
uint8_t hw_usb_transmit(uint8_t *buff);
uint8_t hw_usb_recieve(uint8_t *msg);
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t *len);
void temp(uint8_t *msg, uint8_t dataRecieve, uint32_t dataLength, uint8_t data);

