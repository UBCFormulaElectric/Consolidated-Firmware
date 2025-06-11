#include "io_canLogging.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "app_crc8.h"
#include "cmsis_os.h"
#include "io_canMsg.h"
#include "io_fileSystem.h"
#include "io_log.h"

// Message Queue configuration
#define QUEUE_SIZE 2000
#define PATH_LENGTH 64
#define QUEUE_BYTES (sizeof(CanMsg) * QUEUE_SIZE)

#define CHECK_ENABLED()                                    \
    if (logging_error_remaining == 0)                      \
    {                                                      \
        LOG_ERROR("CAN logging disabled, not continuing"); \
        return;                                            \
    }

#define CHECK_ERR(f)                                                  \
    if (!(f))                                                         \
    {                                                                 \
        logging_error_remaining--;                                    \
        LOG_ERROR("CAN logging error! Remaining errors decremented"); \
        return;                                                       \
    }

#define CHECK_ERR_CRITICAL(f)                                           \
    if (!(f))                                                           \
    {                                                                   \
        logging_error_remaining = 0;                                    \
        LOG_ERROR("Critical CAN logging failure, logging unavailable"); \
        return;                                                         \
    }

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

void io_canLogging_init(char *file_name_prefix)
{
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);
    assert(message_queue_id != NULL);

    // Initialize the filesystem.
    CHECK_ERR_CRITICAL(io_fileSystem_init() == FILE_OK);

    // create new file for this boot
    CHECK_ERR_CRITICAL(io_fileSystem_getBootCount(&current_bootcount) == FILE_OK);

    if (file_name_prefix == NULL)
        snprintf(current_path, sizeof(current_path), "/%03lu.txt", current_bootcount);
    else
        snprintf(current_path, sizeof(current_path), "/%s_%03lu.txt", file_name_prefix, current_bootcount);

    // Open the log file
    CHECK_ERR_CRITICAL(io_fileSystem_open(current_path, &log_fd) == FILE_OK);
}
void io_canLogging_recordMsgFromQueue(void)
{
    CHECK_ENABLED();

    // Assert here since we pass "wait forever" for a message. So it'll only fail if we've configured something wrong.
    CanMsg msg = { 0 };
    assert(osMessageQueueGet(message_queue_id, &msg, NULL, osWaitForever) == osOK);

    // Message log payload:
    // 1. Magic number 0xBA (1 byte)
    // 2. CRC8 checksum (1 byte)
    // 3. Timestamp (2 bytes)
    // 4. ID (4 bytes)
    // 5. DLC code (1 byte)
    // 6. Data bytes (0-64 bytes)
    // (9-73 bytes total)

    const uint8_t  magic     = 0xBA;
    const uint16_t timestamp = (uint16_t)msg.timestamp;
    const uint32_t id        = msg.std_id;
    const uint8_t  dlc       = (uint8_t)msg.dlc;

    uint8_t crc = app_crc8_init();
    crc         = app_crc8_update(crc, &timestamp, sizeof(timestamp));
    crc         = app_crc8_update(crc, &id, sizeof(id));
    crc         = app_crc8_update(crc, &dlc, sizeof(dlc));
    crc         = app_crc8_update(crc, &msg.data.data8, dlc);
    crc         = app_crc8_finalize(crc);

    CHECK_ERR(io_fileSystem_write(log_fd, &magic, sizeof(magic)) == FILE_OK);
    CHECK_ERR(io_fileSystem_write(log_fd, &crc, sizeof(crc)) == FILE_OK);
    CHECK_ERR(io_fileSystem_write(log_fd, &timestamp, sizeof(timestamp)) == FILE_OK);
    CHECK_ERR(io_fileSystem_write(log_fd, &id, sizeof(id)) == FILE_OK);
    CHECK_ERR(io_fileSystem_write(log_fd, &dlc, sizeof(dlc)) == FILE_OK);
    CHECK_ERR(io_fileSystem_write(log_fd, msg.data.data8, dlc) == FILE_OK);
}

void io_canLogging_loggingQueuePush(const CanMsg *rx_msg)
{
    CHECK_ENABLED();

    static uint32_t overflow_count = 0;

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    CHECK_ERR(osMessageQueuePut(message_queue_id, &rx_msg, 0, 0) == osOK);
}

void io_canLogging_sync(void)
{
    CHECK_ENABLED();
    CHECK_ERR(io_fileSystem_sync(log_fd) == FILE_OK);
}

uint32_t io_canLogging_getCurrentLog(void)
{
    return current_bootcount;
}

uint32_t io_canLogging_errorsRemaining(void)
{
    return logging_error_remaining;
}
