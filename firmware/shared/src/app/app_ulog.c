#include "app_ulog.h"
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

static UlogQueue ulog_queue = { .queue = {}, .dequeue_index = 0, .enqueue_index = 0 };

void app_ulog_init(void (*can_log_tx)(char[ULOG_CHUNK_SIZE]), void (*can_overflow_warning)(bool))
{
    ulog_queue.can_log_tx           = can_log_tx;
    ulog_queue.can_overflow_warning = can_overflow_warning;
}

void app_ulog_enqueue(char chunk[ULOG_CHUNK_SIZE])
{
    // copy from stack to static memory.
    memcpy(ulog_queue.queue[ulog_queue.enqueue_index], chunk, ULOG_CHUNK_SIZE);

    ulog_queue.enqueue_index += 1;
    ulog_queue.enqueue_index %= MAX_ULOG_QUEUE_LENGTH;

    if (ulog_queue.enqueue_index == ulog_queue.dequeue_index)
    {
        ulog_queue.can_overflow_warning(true);
    }
}

void app_ulog_log(char msg[])
{
    int          msg_len     = strlen(msg);
    int          extra_chunk = msg_len % ULOG_CHUNK_SIZE != 0;
    unsigned int chunk_count = msg_len / ULOG_CHUNK_SIZE + extra_chunk;

    // pad with nil chars to the nearest multiple of 8
    int  padded_len = chunk_count * ULOG_CHUNK_SIZE;
    char padded_msg[padded_len];
    for (int i = 0; i < padded_len; i += 1)
    {
        padded_msg[i] = i < msg_len ? msg[i] : '\0';
    }

    // chunk out string and enqueue
    for (int chunk_index = 0; chunk_index < chunk_count; chunk_index += 1)
    {
        app_ulog_enqueue(&padded_msg[chunk_index * ULOG_CHUNK_SIZE]);
    }
}

char *app_ulog_dequeue()
{
    // don't allow dequeues on an empty queue
    if (ulog_queue.enqueue_index == ulog_queue.dequeue_index)
    {
        return NULL;
    }

    char *res = (ulog_queue.queue)[ulog_queue.dequeue_index];
    ulog_queue.dequeue_index += 1;
    ulog_queue.dequeue_index %= MAX_ULOG_QUEUE_LENGTH;

    return res;
}

void app_ulog_broadcast()
{
    char *chunk = app_ulog_dequeue();

    if (chunk != NULL)
    {
        // convert chars to uint32
        uint32_t chunkBits;
        memcpy(&chunkBits, chunk, ULOG_CHUNK_SIZE);

        ulog_queue.can_log_tx(chunkBits);
    }
    else
    {
        ulog_queue.can_log_tx(0);
    }
}