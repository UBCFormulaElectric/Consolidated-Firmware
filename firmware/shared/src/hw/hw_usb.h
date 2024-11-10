#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"//usb is LL API so idk if I'd need this 
#include "usbd_cdc.h"

typedef struct
{
    // USB_ModeTypeDef *const mode; // pointer to structure containing USB Mode
    // USB_URBStateTypeDef *const state; //pointer to the URB type
    // USB_HCStateTypeDef *const hostState; //pointer to the host state
    // USB_CfgTypeDef *const config; //usb configuration type
    // USB_EPTypeDef *const endpoint; //endpoint type
    // USB_HCTypeDef *const hostType; // host type
    USBD_CDC_ItfTypeDef *const handle;
} USB;

static int8_t USB_CDC_Init(void);
uint8_t USB_CDC_Transmit(USB *usb);
uint8_t USB_CDC_Receive(USB *usb);




