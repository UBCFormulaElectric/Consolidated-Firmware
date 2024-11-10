#include "hw_usb.h" 

uint8_t hw_CDC_TxBuffer(USB *usb, uint8_t *pbuff, uint32_t length){
    USBD_CDC_SetTxBuffer(usb->handle, pbuff, length, usb->ClassId);
}

uint8_t hw_CDC_transmitPacket(USB *usb){
    USBD_CDC_TransmitPacket(usb->handle, usb->ClassId);
}

uint8_t hw_CDC_setRXBuffer(USB *usb, uint8_t *pbuff){
    USBD_CDC_SetRxBuffer(usb, pbuff);
}

uint8_t hw_CDC_RecievePacket(USB *usb){
    USBD_CDC_ReceivePacket(usb);
}
