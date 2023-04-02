#include <assert.h>
#include <string.h>

#include "App_RingQueue.h"

void App_RingQueue_Init(RingQueue *queue, int size)
{
    // Queue size cannot exceed allocated buffer size
    assert(size > 0 && size <= RING_QUEUE_MAX_SIZE);
    assert(queue);

    memset(queue->elements, 0U, sizeof(queue->elements));
    queue->size  = size;
    queue->head  = 0;
    queue->tail  = -1;
    queue->count = 0;
}

void App_RingQueue_Push(RingQueue *queue, uint8_t value)
{
    assert(queue);

    // Increment tail idx, with wrapping if necesssary
    queue->tail += 1;
    queue->tail %= queue->size;

    // Insert the new value
    queue->elements[queue->tail] = value;

    // The queue has overflowed if the tail idx has "circled around" and caught up to the head idx,
    // so increment the head idx. Also check there are elements in queue so this is not set for
    // an empty queue (which has tail and head idx equal).
    const bool queue_overflowed = (queue->tail == queue->head) && queue->count > 0;
    if (queue_overflowed)
    {
        // Count doesn't change if overflowed, since a new element replaced an old element
        queue->head += 1;
        queue->head %= queue->size;
    }
    else
    {
        // Queue has space, so we increase the number of elements by 1
        queue->count += 1;
    }
}

bool App_RingQueue_Pop(RingQueue *queue, uint8_t *value)
{
    assert(queue);
    assert(value);

    if (queue->count == 0)
    {
        // Queue is empty, return false
        return false;
    }

    // Write value at head idx to output
    *value = queue->elements[queue->head];

    // Move head to next-oldest value for future pop operations, and shrink size by 1
    queue->head += 1;
    queue->head %= queue->size;
    queue->count -= 1;
    return true;
}

int App_RingQueue_Count(RingQueue *queue)
{
    assert(queue);
    return queue->count;
}
