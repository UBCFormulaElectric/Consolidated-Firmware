#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include "io_canLoggingQueue.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "cmsis_os.h"
#include "io_fileSystem.h"

// batch message buffer
#define BATCH_SIZE 10
struct CanMsgBatch {
    CanMsgLog msg[BATCH_SIZE];
};
static struct CanMsgBatch batch_buf;
static uint32_t           batch_count = 0;

// Message Queue configuration
#define QUEUE_SIZE 256
#define PATH_LENGTH 8
#define QUEUE_BYTES sizeof(CanMsgLog) * QUEUE_SIZE* BATCH_SIZE

// State
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];
static uint32_t           current_bootcount;
static int                log_fd; // fd for the log file
static char               current_path[10];
static uint8_t            logging_error_remaining = 10; // number of times to error before stopping logging

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

static void convertCanMsgToLog(const CanMsg* msg, CanMsgLog* log) {
    log->id        = msg->std_id;
    log->dlc       = msg->dlc;
    log->timestamp = msg->timestamp;
    memcpy(log->data, msg->data, 8);
}

static bool isLoggingEnabled(void) {
    return logging_error_remaining > 0;
}

// Push a message to the buffer
// return true and reset the batch counter if the buffer is full after pushing
// return false otherwise
static bool pushToBuffer(const CanMsgLog* msg) {
    // the buffer should always have space for the message
    assert(batch_count < BATCH_SIZE);
    batch_buf.msg[batch_count] = *msg;
    batch_count++;
    if (batch_count >= BATCH_SIZE) {
        batch_count = 0;
        return true;
    }
    return false;
}

int io_canLogging_init() {
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg) * BATCH_SIZE, &queue_attr);

    // create new folder for this boot
    current_bootcount = io_fileSystem_getBootCount();
    sprintf(current_path, "/%lu.txt", current_bootcount);
    log_fd = io_fileSystem_open(current_path);
    if (log_fd < 0) {
        logging_error_remaining = 0;
        return 1;
    }
    return 0;
}

int io_canLogging_recordMsgFromQueue(void) {
    if (!isLoggingEnabled())
        return 1;

    struct CanMsgBatch tx_msg;
    osMessageQueueGet(message_queue_id, &tx_msg, NULL, osWaitForever);

    if (io_fileSystem_write(log_fd, &tx_msg, sizeof(tx_msg.msg)) < 0 && logging_error_remaining > 0) {
        logging_error_remaining--;
        return 1;
    }
    return 0;
}

bool io_canLogging_loggingQueuePush(const CanMsg* rx_msg) {
    if (!isLoggingEnabled())
        return false;

    static uint32_t overflow_count = 0;
    CanMsgLog       msg_log;
    convertCanMsgToLog(rx_msg, &msg_log);

    // cache the message to the buffer first
    if (!pushToBuffer(&msg_log))
        return false;

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    return osMessageQueuePut(message_queue_id, &batch_buf.msg, 0, 0) == osOK;
}

int io_canLogging_sync(void) {
    if (!isLoggingEnabled())
        return 1;

    // Save the seek before close
    if (io_fileSystem_sync(log_fd) < 0 && logging_error_remaining > 0) {
        logging_error_remaining--;
        return 1;
    }
    return 0;
}

uint32_t io_canLogging_getCurrentLog(void) {
    return current_bootcount;
}

uint32_t io_canLogging_errorsRemaining(void) {
    return logging_error_remaining;
}

#pragma GCC diagnostic pop