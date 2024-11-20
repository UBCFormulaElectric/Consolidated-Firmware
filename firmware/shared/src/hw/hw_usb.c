#include "hw_usb.h" 

#define RX_QUEUE_SIZE 10;

// Private globals.

// memory for control block
// Pointer to a memory for the message queue control block object. Refer to Static Object Memory for more information.
// Default: NULL to use Automatic Dynamic Allocation for the message queue control block.

static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_SIZE];
static osMessageQueueId_t rx_queue_id;

static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t), //this is for the control block like usb control which we don't use so we init but no use! 
    .mq_mem    = rx_queue_buf, //this  is the size of indivdual data 
    .mq_size   = sizeof(rx_queue_buf) //max allocated memory needed 
};

osMessageQueueId_t hw_usb_init(){
    //should I pass in MAX_MSG_SIZE or sizeof(uint8_t)? decided MAX for now bc protobuf is 32 at once
    rx_queue_id = osMessageQueueNew(sizeof(uint8_t), RX_QUEUE_SIZE, &rx_queue_attr);
    return rx_queue_id;
}

uint8_t hw_usb_transmit(uint8_t *buff){
    return CDC_Transmit_FS(buff, len(buff));
}

//hw_usb_recieve
uint8_t hw_usb_recieve(uint8_t msg){
    if (osMessageQueueGetSpace (rx_queue_id) == RX_QUEUE_SIZE){
        printf("queue is empty");
    }
    osStatus_t s = osMessageQueueGet(rx_queue_id, msg, NULL, osWaitForever);
    // Pop a message off the RX queue. rn the message priority is NULL
    assert(s == osOK);
    return msg;
}

void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t *len){
    // message on the RX queue. wrapping it to get a void pointer cuz osMessageQUeuePut takes in a void pointer
    uint32_t space = osMessageQueueGetSpace (rx_queue_id);
    if(len > space){
        printf("not enough space in the queue");
    }
    else{
        osStatus_t s = osMessageQueuePut(rx_queue_id, &packet, NULL, osWaitForever);
        assert(s == osOK);
    } 
}
