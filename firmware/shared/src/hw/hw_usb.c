#include "hw_usb.h" 

// Private globals.
static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_SIZE];
static osMessageQueueId_t rx_queue_id = NULL;
static USBD_HandleTypeDef hUsbDeviceFS;

static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block, 
    .cb_size   = sizeof(StaticQueue_t), 
    .mq_mem    = rx_queue_buf, 
    .mq_size   = sizeof(rx_queue_buf) 
};

//initalizing a queue
void hw_usb_init(){
    if (rx_queue_id == NULL) {
        rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(uint8_t), &rx_queue_attr);
    }
}

//checking connection 
bool hw_usb_checkConnection(){
    return hUsbDeviceFS.dev_state != USBD_STATE_SUSPENDED;
}

//trasmitting a message
void hw_usb_transmit(uint8_t *msg, uint16_t len){
    LOG_INFO("Return status: %d", CDC_Transmit_FS(msg, len));
}

//reading the message sent to the board from the queue
uint8_t hw_usb_recieve(){
    uint8_t msg = 0;
    osStatus_t status = osMessageQueueGet(rx_queue_id, &msg, NULL, 100);
    if (status == osErrorTimeout) return 0;
    else if (status == osOK) return msg;
    
    return 0;
}

//when packet recieved placing the message into the queue one byte at a time
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len){
    uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    LOG_INFO("we are checking the space %lu", (unsigned long)space);
    assert(len < space);
    for (uint32_t i = 0; i < len; i += 1) {
        osStatus_t status = osMessageQueuePut(rx_queue_id, &packet[i], 0, 0); 
    }
    LOG_INFO("Message entered into the queue");
}

//example to test transmitting information and recieving it on the python server
void hw_usb_transmit_example() {
    hw_usb_init();
    LOG_INFO("queue got initialized...");
    assert (rx_queue_id != NULL);
    osDelay(1000);
    int packet_count = 0;
    for (;;){
        LOG_INFO("calling usb transmit");
        uint8_t *packet = (uint8_t *) "hello";
        //need to decide if we want to add a null indicator at the end
        hw_usb_transmit(packet, 5);
        packet_count++;
        LOG_INFO("transmitted packet %d times", packet_count);
        osDelay(1000);
    }
}

//example of reading the recieved msg from teh queue
void hw_usb_recieve_example() {
    hw_usb_init();
    for (int i = 0; true; i += 1) {
        uint8_t result = hw_usb_recieve();
        LOG_INFO("char: %c", (char) result);
        osDelay(1000);
    }
}
