#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_QUEUE_MAX_SIZE 4096

// Data struct for a ring queue: Do not interact with member vars directly!
typedef struct
{
    uint8_t elements[RING_QUEUE_MAX_SIZE];
    int  size;
    int  head;
    int  tail;
    int  count;
} RingQueue;

/**
 * Initialize a ring queue.
 * @param queue: Queue to initialize
 * @param size: Desired size of queue, cannot exceed RING_QUEUE_MAX_SIZE
 */
void App_RingQueue_Init(RingQueue *queue, int size);

/**
 * @brief Push to queue. Will overwrite the oldest value if the ring buffer has
 *        exceeded its size.
 * @param queue: Queue to push to
 * @param value: Value to push
 */
void App_RingQueue_Push(RingQueue *queue, uint8_t value);

/**
 * Pop from queue.
 * @param queue: Queue to pop from
 * @param value: Pointer to the popped output
 * @return True if an item was popped off, false otherwise (i.e. if queue was empty)
 */
bool App_RingQueue_Pop(RingQueue *queue, uint8_t *value);

// /**
//  * Copy the queue's contents to an array, ordered from oldest to most
//  *        recent element. Writes all the elements currently in the queue.
//  * @param queue: Queue to read from
//  * @param array: Array to write to, must have enough space to receive all of the
//  *               queue's elements!
//  */
// void App_RingQueue_CopyToArray(RingQueue *queue, float *array);

/**
 * Get the number of elements in a queue.
 * @param queue: Queue to read from
 */
int App_RingQueue_Count(RingQueue *queue);