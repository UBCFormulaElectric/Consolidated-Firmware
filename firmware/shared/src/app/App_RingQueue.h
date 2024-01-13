#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_QUEUE_MAX_SIZE 4096 // 4kB

// Data struct for a ring queue: Do not interact with member vars directly!

typedef struct
{
    void (*const ring_queue_overflow_callback)(bool); // callback on ring queue overflow
}sensor_msg_config;


/**
 * Initialize a ring queue and sensor message peripheral
 * @param msg_config config struct.
 */
 */
void App_RingQueue_Init(const sensor_msg_config *sensor_msg_config);

/**
 * Pushes an SBG Ellipse sensor message into Queue.
 * Does not block, calls `ring_queue_overflow_callback` if queue is full.
 * @param msg sensor msg to be TXed.
 */
void App_RingQueue_Push(const uint8_t *value);

/**
 * Check if there is data in the queue and if so pop first data in queue 
 */
bool App_RingQueue_Pop(uint8_t *value);
