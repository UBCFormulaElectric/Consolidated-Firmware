#include "hw_usb.h" 

// Private globals.

// memory for control block
// Pointer to a memory for the message queue control block object. Refer to Static Object Memory for more information.
// Default: NULL to use Automatic Dynamic Allocation for the message queue control block.

static StaticQueue_t      rx_queue_control_block;
static uint8_t            rx_queue_buf[RX_QUEUE_SIZE];
static osMessageQueueId_t rx_queue_id = NULL;

static const osMessageQueueAttr_t rx_queue_attr = {
    .name      = "USB RX Queue",
    .attr_bits = 0,
    .cb_mem    = &rx_queue_control_block, //or should I do NULL and size 0, right now RTOS is dynamically allocating memroy not with 
    .cb_size   = sizeof(StaticQueue_t), //this is for the control block like usb control which we don't use so we init but no use! 
    .mq_mem    = rx_queue_buf, //this  is the size of indivdual data 
    .mq_size   = sizeof(rx_queue_buf) //max allocated memory needed 
};

//make this a void maybe?

osMessageQueueId_t hw_usb_init(){
    //should I pass in MAX_MSG_SIZE or sizeof(uint8_t)? decided MAX for now bc protobuf is 32 at once
    if (rx_queue_id == NULL){
        rx_queue_id = osMessageQueueNew(RX_QUEUE_SIZE, sizeof(uint8_t), &rx_queue_attr);
    }
    return rx_queue_id;
}

void hw_usb_transmit(uint8_t *msg, uint16_t len){
    assert(CDC_Transmit_FS(msg, len) == USBD_OK);
}

uint8_t hw_usb_recieve(){
    // Pop a message off the RX queue. rn the message priority is NULL
    uint8_t *msg;
    osStatus_t status = osMessageQueueGet(rx_queue_id, msg, NULL, 100);
    if (status == osErrorTimeout) return 0;
    else if (status == osOK) return *msg;
    
    return 0;
}

//since packet is a POINTER!!!! It's acc pointing to an array that stores 8 bit values!!! if it was uint packet then its just 1 thing!! everyting makes sense NOW!
void hw_usb_pushRxMsgToQueue(uint8_t *packet, uint32_t len){
    // message on the RX queue. wrapping it to get a void pointer cuz osMessageQUeuePut takes in a void pointer
    
    uint32_t space = osMessageQueueGetSpace(rx_queue_id);
    assert(len < space);
    for (int i = 0; i < len; i += 1) {
        osStatus_t s = osMessageQueuePut(rx_queue_id, &packet[i], NULL, osWaitForever); //oswaitforever is okay cuz eventually we'll dequeue it waits till queue opens space
        assert(s == osOK);
    }
}
// for the test functionality have an infinite loop that keeps running to see if the rx is 0 once its not zero anymore then we start reaching until were out of bytes 
//update header files and  includes 
void hw_usb_example() {
    LOG_INFO("we are going to call the usb init");
    osMessageQueueId_t rx_queue_id = hw_usb_init();
    LOG_INFO("WE MADE A QUEUEUEUEU");
    assert (rx_queue_id != NULL);
    LOG_INFO("usb initialized! and were gonna send a packet nowwww");
    osDelay(1000);
    const char *packet[] = {"HELLO","HELLO","HELLO"};
    LOG_INFO("packet sent is: %s", *packet);

    //test1
    int i = 0;
    LOG_INFO("we are entering the infinite loop!!");
    for (;;){
        LOG_INFO("calling usb transmit!");
        hw_usb_transmit(packet, sizeof(packet));
        i++;
        LOG_INFO("transmitted packet %d times yay a cycle!", i);
        osDelay(30000);
    }

    //test 2 
    // while (hw_usb_recieve == 0){
    //     hw_usb_recieve();
    // }
    
    // hw_usb_transmit(packet, sizeof(packet));
}
   
   
   
   
   
   
//     osMessageQueueId_t rx_queue_id = hw_usb_init();
//     assert (rx_queue_id != NULL);
//     //LOG_INFO("usb initialized!");
//     osDelay(1000);

//     uint8_t packet[] = {1,2,3,4,5,6};
//     uint8_t recieved_data[sizeof(packet)];
//     uint8_t byte = 0;

//     //LOG_INFO("Starting USB communication test");
//     osDelay(1000);
// //change loop logi
//     while(1){

//         //LOG_INFO("waiting for message to be recieved");
//         osDelay(10000); 
//         hw_usb_transmit(packet, sizeof(packet));
//         assert(osMessageQueueGetCount == 0);
//         osDelay(1000);
//         //LOG_INFO("the packet has been transmitted");

//         for (uint8_t i =0; i<sizeof(packet); i++){
//             byte = hw_usb_recieve();
//             recieved_data[i] = byte; 
//             //LOG_INFO("data is: %i", byte);
//         }
//         //LOG_INFO("testing done!");
//         break;
//     }
// }


//Another option is using SM maybeeee??

// void test_state_machine(){
//     tydef enum{
//         INIT,
//         IDLE,
//         TRANS,
//         RECIEVED,
//         TRANSMITTED
//     } usb_state;

//     usb_state state = INIT;

//     while(1):
//         switch(state){
//             case INIT: 
//                 iolog_init();
//                 osMessageQueueId_t id = hw_usb_init();
//                 assert (rx_queue_id != NULL);
//                 io//LOG_INFO("usb initialized!")
//                 osDelay(1000);

//                 uint8_t packet[] = {1,2,3,4,5,6};
//                 uint8_t recieved_data[sizeof(packet)];
//                 uint8_t byte = 0;

//                 switch IDLE;
//                 break;

//         }

// }