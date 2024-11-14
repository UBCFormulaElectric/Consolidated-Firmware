#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"//usb is LL API so idk if I'd need this 
#include "usbd_cdc_if.h"

typedef struct
{
    // USB_ModeTypeDef *const mode; // pointer to structure containing USB Mode
    // USB_URBStateTypeDef *const state; //pointer to the URB type
    // USB_HCStateTypeDef *const hostState; //pointer to the host state
    // USB_CfgTypeDef *const config; //usb configuration type
    // USB_EPTypeDef *const endpoint; //endpoint type
    // USB_HCTypeDef *const hostType; // host type
    //USBD_CDC_ItfTypeDef *const handle;
    USBD_HandleTypeDef *const handle;
} USB;

void int8_t hw_USB_CDC_Init();
uint8_t hw_USB_CDC_Transmit(uint8_t *buff);
uint8_t hw_USB_CDC_Receive(uint8_t *buff);




