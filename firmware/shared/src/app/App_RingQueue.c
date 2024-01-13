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

    // assert(queue);

    // // Increment tail idx, with wrapping if necesssary
    // queue->tail += 1;
    // queue->tail %= queue->size;

    // // Insert the new value
    // queue->elements[queue->tail] = value;

    // // The queue has overflowed if the tail idx has "circled around" and caught up to the head idx,
    // // so increment the head idx. Also check there are elements in queue so this is not set for
    // // an empty queue (which has tail and head idx equal).
    // const bool queue_overflowed = (queue->tail == queue->head) && queue->count > 0;
    // if (queue_overflowed)
    // {
    //     // Count doesn't change if overflowed, since a new element replaced an old element
    //     queue->head += 1;
    //     queue->head %= queue->size;
    // }
    // else
    // {
    //     // Queue has space, so we increase the number of elements by 1
    //     queue->count += 1;
    // }
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

    // if (queue->count == 0)
    // {
    //     // Queue is empty, return false
    //     return false;
    // }

    // // Write value at head idx to output
    // *value = queue->elements[queue->head];

    // // Move head to next-oldest value for future pop operations, and shrink size by 1
    // queue->head += 1;
    // queue->head %= queue->size;
    // queue->count -= 1;
    // return true;
}
