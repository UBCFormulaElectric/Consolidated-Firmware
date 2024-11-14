#include "hw_usb.h" 

void hw_USB_CDC_Init(){
    CDC_Init_FS();
}

uint8_t hw_USB_CDC_Transmit(uint8_t *pbuff){
    CDC_Transmit_FS(&pbuff, len(pbuff));
}

uint8_t hw_USB_CDC_Receive(uint8_t *pbuff){
    CDC_Receive_FS(&pbuff, len(pbuff));
}

