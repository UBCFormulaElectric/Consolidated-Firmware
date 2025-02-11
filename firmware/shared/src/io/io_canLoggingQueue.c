#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include "io_canLoggingQueue.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "io_fileSystem.h"

// Batch message buffer
#define BATCH_SIZE 10
struct CanMsgBatch
{
    CanMsgLog msg[BATCH_SIZE];
};
static struct CanMsgBatch batch_buf;
static uint32_t           batch_count = 0;

// Message Buffer configuration
#define QUEUE_SIZE 256
#define PATH_LENGTH 8
#define QUEUE_BYTES (sizeof(struct CanMsgBatch) * QUEUE_SIZE)

// State
static MessageBufferHandle_t message_buffer_id;
static StaticMessageBuffer_t buffer_control_block;
static uint8_t               buffer_storage[QUEUE_BYTES];
static uint32_t              current_bootcount;
static int                   log_fd; // fd for the log file
static char                  current_path[10];
static uint8_t               logging_error_remaining = 10; // number of times to error before stopping logging

static void convertCanMsgToLog(const CanMsg *msg, CanMsgLog *log)
{
    log->id        = msg->std_id;
    log->dlc       = msg->dlc;
    log->timestamp = msg->timestamp;
    memcpy(log->data, msg->data, 8);
}

static bool isLoggingEnabled(void)
{
    return logging_error_remaining > 0;
}

int io_canLogging_init()
{
    message_buffer_id = xMessageBufferCreateStatic(QUEUE_BYTES, buffer_storage, &buffer_control_block);
    if (message_buffer_id == NULL)
    {
        logging_error_remaining = 0;
        return 1;
    }

    // create new folder for this boot
    current_bootcount = io_fileSystem_getBootCount();
    sprintf(current_path, "/%lu.txt", current_bootcount);
    log_fd = io_fileSystem_open(current_path);
    if (log_fd < 0)
    {
        logging_error_remaining = 0;
        return 1;
    }
    return 0;
}

int io_canLogging_recordMsgFromQueue(void)
{
    if (!isLoggingEnabled())
        return 1;

    struct CanMsgBatch tx_msg;
    size_t             bytes_received =
        xMessageBufferReceive(message_buffer_id, &tx_msg, sizeof(struct CanMsgBatch), portMAX_DELAY);

    if (bytes_received != sizeof(struct CanMsgBatch))
    {
        assert(0); // Should not fail as we wait indefinitely
        return 1;
    }

    if (io_fileSystem_write(log_fd, &tx_msg.msg, sizeof(tx_msg.msg)) < 0 && logging_error_remaining > 0)
    {
        logging_error_remaining--;
        return 1;
    }
    return 0;
}

bool io_canLogging_loggingQueuePush(const CanMsg *rx_msg)
{
    if (!isLoggingEnabled())
        return false;

    static uint32_t overflow_count = 0;
    CanMsgLog       msg_log;
    convertCanMsgToLog(rx_msg, &msg_log);

    // Add message to batch
    batch_buf.msg[batch_count] = msg_log;
    batch_count++;

    // Check if batch is full
    if (batch_count < BATCH_SIZE)
        return false;

    batch_count = 0;

    // Send the full batch to the message buffer
    size_t bytes_sent = xMessageBufferSend(message_buffer_id, &batch_buf, sizeof(struct CanMsgBatch), 0);
    assert(bytes_sent == sizeof(struct CanMsgBatch));
}

int io_canLogging_sync(void)
{
    if (!isLoggingEnabled())
        return 1;

    // Save the seek before close
    if (io_fileSystem_sync(log_fd) < 0 && logging_error_remaining > 0)
    {
        logging_error_remaining--;
        return 1;
    }
    return 0;
}

uint32_t io_canLogging_getCurrentLog(void)
{
    return current_bootcount;
}

uint32_t io_canLogging_errorsRemaining(void)
{
    return logging_error_remaining;
}

#pragma GCC diagnostic pop