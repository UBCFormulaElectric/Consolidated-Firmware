#pragma GCC diagnostic ignored "-Wconversion"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"
#include "hw_gpio.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_time.h"

// Private globals.
static const CanConfig *config;
#define QUEUE_SIZE 2048
#define QUEUE_BYTES sizeof(CanMsgLog) * QUEUE_SIZE
#define PATH_LENGTH 16
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];

static uint32_t current_bootcount;
static int      log_fd; // fd for the log file

static uint8_t logging_error_remaining = 10; // number of times to error before stopping logging

static char current_path[10];

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

// assume the filesystem is already inited
static int initLoggingFileSystem(void)
{
    current_bootcount = io_fileSystem_getBootCount();

    // create new folder for this boot
    sprintf(current_path, "/%lu.txt", current_bootcount);
    log_fd = io_fileSystem_open(current_path);
    if (log_fd < 0)
    {
        logging_error_remaining = 0;
        return 1;
    }

    return 0;
}

static void convertCanMsgToLog(const CanMsg *msg, CanMsgLog *log)
{
    log->id        = msg->std_id;
    log->dlc       = msg->dlc;
    log->timestamp = io_time_getCurrentMs();
    memcpy(log->data, msg->data, 8);
}

static bool isLoggingEnabled(void)
{
    return logging_error_remaining > 0 && config != NULL;
}

int io_canLogging_init(const CanConfig *can_config)
{
    assert(can_config != NULL);
    if (config != NULL)
    {
        return 1; // return 1 if already initialized
    }

    config = can_config;

    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);

    // create new folder for this boot
    return initLoggingFileSystem();
}

int io_canLogging_recordMsgFromQueue(void)
{
    if (!isLoggingEnabled())
    {
        return 1;
    }

    CanMsgLog tx_msg;
    osMessageQueueGet(message_queue_id, &tx_msg, NULL, osWaitForever);

    int err = io_fileSystem_write(log_fd, &tx_msg, sizeof(tx_msg));
    if (err < 0 && logging_error_remaining > 0)
    {
        logging_error_remaining--;
        return 1;
    }
    return 0;
}

void io_canLogging_loggingQueuePush(CanMsg *msg)
{
    if (!isLoggingEnabled())
    {
        return;
    }

    if (config->rx_msg_filter != NULL && !config->rx_msg_filter(msg->std_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    static uint32_t overflow_count = 0;
    CanMsgLog       msg_log;
    convertCanMsgToLog(msg, &msg_log);

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(message_queue_id, &msg_log, 0, 0) != osOK && config->rx_overflow_callback != NULL)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        config->rx_overflow_callback(++overflow_count);
    }
}

int io_canLogging_sync(void)
{
    if (!isLoggingEnabled())
    {
        return 1;
    }

    // SAVe the seek before close
    int err = io_fileSystem_sync(log_fd);
    if (err < 0 && logging_error_remaining > 0)
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