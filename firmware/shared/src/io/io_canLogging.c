#include "io_canLogging.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"

// Private globals.
static const CanConfig *config;
#define QUEUE_SIZE 20
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define PATH_LENGTH 10
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];

#define QUEUE_SIZE 20
static uint32_t current_bootcount;

static lfs_t      lfs;
static lfs_file_t file;

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

// assume the lfs is already mounted
static void createFolder(struct lfs_config *cfg)
{
    // get bootcount
    uint32_t bootcount = 0;
    lfs_file_open(&lfs, &file, "bootcount", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &bootcount, sizeof(bootcount));

    // update bootcount
    bootcount += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &bootcount, sizeof(bootcount));
    current_bootcount = bootcount;

    // close the bootcount file
    lfs_file_close(&lfs, &file);

    // create new folder on root based on the bootcount
    const char path[PATH_LENGTH];
    sprintf((char *)path, "%lu", bootcount);
    lfs_mkdir(&lfs, path);
}

void io_canLogging_init(const CanConfig *can_config, struct lfs_config *cfg)
{
    assert(can_config != NULL);
    config = can_config;

    // Initialize CAN queues.
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);

    // create new folder for this boot
    createFolder(cfg);
}

void io_canLogging_pushTxMsgToQueue(const CanMsg *msg)
{
    static uint32_t tx_overflow_count = 0;

    if (osMessageQueuePut(message_queue_id, msg, 0, 0) != osOK && config->tx_overflow_callback != NULL)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the TX overflow callback.
        config->tx_overflow_callback(++tx_overflow_count);
    }
}

void io_canLogging_recordMsgFromQueue(void)
{
    CanMsg tx_msg;
    osMessageQueueGet(message_queue_id, &tx_msg, NULL, osWaitForever);
    // write the message to the file system
}

void io_canLogging_msgReceivedCallback(uint32_t rx_fifo)
{
    static uint32_t rx_overflow_count = 0;

    CanMsg rx_msg;
    if (!hw_can_receive(rx_fifo, &rx_msg))
    {
        // Early return if RX msg is unavailable.
        return;
    }

    if (config->rx_msg_filter != NULL && !config->rx_msg_filter(rx_msg.std_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(message_queue_id, &rx_msg, 0, 0) != osOK && config->rx_overflow_callback != NULL)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        config->rx_overflow_callback(++rx_overflow_count);
    }
}