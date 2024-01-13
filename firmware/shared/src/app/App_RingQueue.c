#include <assert.h>
#include <string.h>
#include "cmsis_os.h"
#include "queue.h"
#include "App_RingQueue.h"

#define QUEUE_SIZE RING_QUEUE_MAX_SIZE/sizeof(uint8_t)

static const sensor_msg_config *config;


static osMessageQueueId_t ring_queue_id;
static StaticQueue_t      ring_queue_control_block;
static uint8_t            ring_queue_buf[RING_QUEUE_MAX_SIZE];

static const osMessageQueueAttr_t ring_queue_attr = {
    .name      = "RingQueue",
    .attr_bits = 0,
    .cb_mem    = &ring_queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = ring_queue_buf,
    .mq_size   = RING_QUEUE_MAX_SIZE,
};

void App_RingQueue_Init(const sensor_msg_config *msg_config)
{
    assert(msg_config != NULL);
    config = msg_config;

   ring_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(uint8_t), &ring_queue_attr);

   assert(ring_queue_id != NULL);

}

void App_RingQueue_Push(const uint8_t *value)
{
    static uint32_t ring_queue_overflow_count = 0;

    if (osMessageQueuePut(ring_queue_id, value, 0 , 0) != osOK)
    {
         config->ring_queue_overflow_callback(++ring_queue_overflow_count);
    }

}

bool App_RingQueue_Pop(uint8_t *value)
{
    assert(value);

    // Pop a message off the RX queue.
        if (osMessageQueueGetCount(ring_queue_id) == 0) {
        return false;
    }
     osMessageQueueGet(ring_queue_id, value, NULL, osWaitForever);
    return true;

}
