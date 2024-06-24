#pragma once
#include <stdbool.h>
#include <stdint.h>

#define MAX_ULOG_QUEUE_LENGTH (1000U)
#define ULOG_CHUNK_SIZE (4U) // 4 bytes = 32 bits = max can signal size

// a cyclical queue, used for storing chunks of a log to be dequeued and broadcast at the users discretion
typedef struct
{
    // an array of chunks of strings to log, held continguously.
    char         queue[ULOG_CHUNK_SIZE][MAX_ULOG_QUEUE_LENGTH];
    unsigned int dequeue_index;
    unsigned int enqueue_index;
    void (*can_log_tx)(uint32_t);
    void (*can_overflow_warning)(bool);
} UlogQueue;

/**
 * Initialize the unstructured logger.
 * @param can_log_tx The tx callback to call when a chunk is ready to be broadcasted.
 * @param can_overflow_warning The warning callback to call if an overflow is detected.
 */
void app_ulog_init(void (*can_log_tx)(uint32_t), void (*can_overflow_warning)(bool));

/**
 * Log a message through the unstructured logging system, and trigger can_overflow_warning if an overflow is detected.
 * @param msg The message to log.
 */
void app_ulog_log(char msg[]);

/**
 * Broadcast the next chunk over can if it is available.
 */
void app_ulog_broadcast();