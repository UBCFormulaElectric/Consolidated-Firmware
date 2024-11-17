#include "hw_usb.h" 
#include "hw_usbMsgQueue.h"
#include "usbd_cdc_if.h"

#define RX_QUEUE_BYTES 20;
#define MAX_MSG_SIZE 32; 
// Private globals.
static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_BYTES];
static const osMessageQueueId_t rx_queue_id;

static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = rx_queue_buf,
    .mq_size   = RX_QUEUE_BYTES,
};

void hw_USB_CDC_Init(){
    //should I pass in MAX_MSG_SIZE or sizeof(uint8_t)?
    rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, MAX_MSG_SIZE , &rx_queue_attr);
}

uint8_t hw_USB_CDC_Transmit(uint8_t *pbuff){
    CDC_Transmit_FS(pbuff, len(pbuff));
}

uint8_t hw_USB_CDC_Receive(uint8_t *pbuff){
    CDC_Receive_FS(pbuff, len(pbuff));
}

void hw_usb_popRxMsgFromQueue(uint8_t *pbuff){
    // Pop a message off the RX queue. rn the message priority in NULL but we can chnage that
    const osStatus_t s = osMessageQueueGet(rx_queue_id, pbuff, NULL, osWaitForever);
    assert(s == osOK);
}

void hw_usb_pushRxMsgToQueue(USBD_HandleTypeDef *pdev){
    // message on the RX queue.
    uint8_t rx_msg = USBD_CDC_ReceivePacket(pdev);
    uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    if (space >= sizeof(rx_msg)){
    const osStatus_t s = osMessageQueuePut(rx_queue_id, rx_msg, NULL, osWaitForever);
    assert(s == osOK);
    }

    else{
        //will change this 
        osMessageQueueReset(rx_queue_id);
    }
}
