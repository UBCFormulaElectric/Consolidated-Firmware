#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"//usb is LL API so idk if I'd need this 
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "usbd_cdc.h"
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

void hw_USB_CDC_Init();
uint8_t hw_USB_CDC_Transmit(uint8_t *pbuff);
uint8_t hw_USB_CDC_Receive(uint8_t *pbuff);
void hw_usb_popRxMsgFromQueue(uint8_t *pbuff);
void hw_usb_pushRxMsgToQueue(USBD_HandleTypeDef *pdev);
