#include "io_telemMessage.h"
#include "cmsis_os.h"
#include "queue.h"

// use a static queue for better memory management
// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such

// Private Globals
extern bool Modem900;

#define QUEUE_SIZE 2048
#define QUEUE_BYTES 4 * QUEUE_SIZE // this is all temp

static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];

static const osMessageQueueAttr_t queue_attr = {
    .name      = "Telem Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

void io_telemMessage_init()
{
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, 4, &queue_attr);
    if (message_queue_id == NULL)
    {
        ; // Message Queue object not created, handle failure
    }
}

void io_telemMessage_pushMsgToQueue(const CanMsg *msg)
{
    // take the can msg
    // check if it is in the 'truth tale' of msgs that we are interested in
    // serialize it
    // push this serialization to the queue
    osMessageQueuePut(message_queue_id, &msg, 0U, 0U);
    return;
}

void io_telemMessage_popMsgFromQueue()
{
    // pop a queue msg
    // send it over the correct UART functionality
    return;
}