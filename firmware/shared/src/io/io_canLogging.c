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
    // 2. DLC code (1 byte)
    // 3. Timestamp (2 bytes)
    // 4. ID (4 bytes)
    // 5. Data bytes (0-64 bytes)
    // 6. CRC8 checksum (1 byte)

    // Construct log header.
    struct
    {
        uint8_t  magic;
        uint8_t  dlc;
        uint16_t timestamp;
        uint32_t id;
    } header = { .magic = 0xBA, .dlc = (uint8_t)msg.dlc, .timestamp = (uint16_t)msg.timestamp, .id = msg.std_id };
    static_assert(sizeof(header) == 8, "Header is exactly 8 bytes!");

    // Calculate CRC8 for error detetction.
    uint8_t crc = app_crc8_init();
    crc         = app_crc8_update(crc, &header, sizeof(header));
    crc         = app_crc8_update(crc, msg.data.data8, msg.dlc);
    crc         = app_crc8_finalize(crc);

    // Copy to a buffer to write to the file.
    uint8_t buf[sizeof(header) + msg.dlc + sizeof(uint8_t)];
    memcpy(buf, &header, sizeof(header));
    memcpy(buf + sizeof(header), msg.data.data8, msg.dlc);
    memcpy(buf + sizeof(buf) - 1, &crc, sizeof(uint8_t));

    CHECK_ERR(io_fileSystem_write(log_fd, &buf, sizeof(buf)) == FILE_OK);
}

void io_canLogging_loggingQueuePush(const CanMsg *rx_msg)
{
    CHECK_ENABLED();

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    CHECK_ERR(osMessageQueuePut(message_queue_id, rx_msg, 0, 0) == osOK);
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
