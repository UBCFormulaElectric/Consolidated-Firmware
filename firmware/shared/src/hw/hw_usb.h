#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"//usb is LL API so idk if I'd need this 
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include <string.h>
#include <assert.h>
#include "hw_usb.h"

// Sizes of TX and RX queues.
#define RX_QUEUE_SIZE 256
#define RX_QUEUE_BYTES sizeof(pbuff) * RX_QUEUE_SIZE

// typedef struct
// {
//     /*initiating a rx queue */
//     osMessageQueueId_t rx_queue_id;
// } USB;

void int8_t hw_USB_CDC_Init();
uint8_t hw_USB_CDC_Transmit(uint8_t *buff);
uint8_t hw_USB_CDC_Receive(uint8_t *buff);
void hw_usbQueue_init(const USBD_HandleTypeDef &hUsbDeviceFS);
// void hw_usb_pushTxMsgToQueue(const uint8_t *pbuff);
// void hw_usb_popTxMsgFromQueue(uint8_t *pbuff);
// void hw_usb_transmitMsgFromQueue(uint8_t *pbuff);
void hw_usb_popRxMsgFromQueue(uint8_t *pbuff);
void hw_usb_pushRxMsgToQueue(uint8_t *pbuff);
