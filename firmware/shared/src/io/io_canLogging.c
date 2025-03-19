#include "io_canLogging.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "cmsis_os.h"
#include "io_fileSystem.h"

// Message Queue configuration
#define QUEUE_SIZE 2000
#define PATH_LENGTH 64
#define QUEUE_BYTES (sizeof(CanMsgLog) * QUEUE_SIZE)

// State
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];
static uint32_t           current_bootcount;
static uint32_t           log_fd; // fd for the log file
static char               current_path[PATH_LENGTH];
static uint8_t            logging_error_remaining = 10; // number of times to error before stopping logging

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

static void convertCanMsgToLog(const CanMsg *msg, CanMsgLog *log)
{
    log->id        = msg->std_id & ID_MASK;
    log->dlc       = msg->dlc & DLC_MASK;
    log->timestamp = msg->timestamp & TIMESTAMP_MASK;
    memcpy(log->data, msg->data, 8);
}

static bool isLoggingEnabled(void)
{
    return logging_error_remaining > 0;
}

bool io_canLogging_init(void)
{
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsgLog), &queue_attr);
    assert(message_queue_id != NULL);

    // Initialize the filesystem.
    FileSystemError err = io_fileSystem_init();
    if (err != FILE_OK)
    {
        logging_error_remaining = 0;
        return false;
    }

    // create new folder for this boot
    err = io_fileSystem_getBootCount(&current_bootcount);
    if (err != FILE_OK)
    {
        logging_error_remaining = 0;
        return false;
    }

    sprintf(current_path, "/%lu.txt", current_bootcount);
    err = io_fileSystem_open(current_path, &log_fd);

    if (err != FILE_OK)
    {
        logging_error_remaining = 0;
        return false;
    }

    return true;
}

bool io_canLogging_recordMsgFromQueue(void)
{
    if (!isLoggingEnabled())
        return false;

    CanMsgLog msg;
    assert(osMessageQueueGet(message_queue_id, &msg, NULL, osWaitForever) == osOK);

    if (io_fileSystem_write(log_fd, &msg, sizeof(msg)) < 0)
    {
        logging_error_remaining--;
        return false;
    }

    return true;
}

bool io_canLogging_loggingQueuePush(const CanMsg *rx_msg)
{
    if (!isLoggingEnabled())
        return false;

    static uint32_t overflow_count = 0;
    CanMsgLog       msg_log;
    convertCanMsgToLog(rx_msg, &msg_log);

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(message_queue_id, &msg_log, 0, 0) != osOK)
    {
        logging_error_remaining--;
        return false;
    }

    return true;
}

bool io_canLogging_sync(void)
{
    if (!isLoggingEnabled())
        return false;

    if (io_fileSystem_sync(log_fd) < 0 && logging_error_remaining > 0)
    {
        logging_error_remaining--;
        return false;
    }

    return true;
}

uint32_t io_canLogging_getCurrentLog(void)
{
    return current_bootcount;
}

uint32_t io_canLogging_errorsRemaining(void)
{
    return logging_error_remaining;
}
