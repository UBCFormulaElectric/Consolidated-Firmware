#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "io/canMsg.h"

// Empirically determined number which corresponds to the time to mount the filesystem, plus the time to create a new
// blank CAN log, taking approximately 500ms. This was found using logfs (not littlefs, there is probably a different
// threshold). The time to mount/create a log both scale linearly with logfs, so for example if there are 400 logs then
// then the time will be ~1s. Proceed with caution creating a significant amount of logs above this threshold.
#define HIGH_NUMBER_OF_LOGS_THRESHOLD (200U)

typedef struct
{
    uint32_t id : 11;
    uint32_t dlc : 4;        // Data length code [0,8]
    uint32_t timestamp : 17; // sum up to 32 bits
    uint8_t  data[8];
    uint8_t  reserved[4]; // reserved byte to round up to 16 bytes, TODO: Use this for error checking?
} CanMsgLog;

#define ID_MASK (0x7FF)          // 11 bits max val
#define DLC_MASK (0xF)           // 4 bits max val
#define TIMESTAMP_MASK (0x1FFFF) // 17 bits max val

/**
 * Create a new message queue for can logging message
 * Create a new file for this boot for record message
 */
void io_canLogging_init(char *file_name_prefix);

/**
 * Populate queue with another item
 */
void io_canLogging_loggingQueuePush(const CanMsg *rx_msg);

/**
 * write the can message to the sdcard
 * pop the massage from queue
 */
void io_canLogging_recordMsgFromQueue(void);

/**
 * Sync all cached stuff to disk.
 */
void io_canLogging_sync(void);

/**
 * Return the number of the current CAN data log.
 */
uint32_t io_canLogging_getCurrentLog(void);

/**
 * Number of remaining errors before logging is disabled.
 */
uint32_t io_canLogging_errorsRemaining(void);
