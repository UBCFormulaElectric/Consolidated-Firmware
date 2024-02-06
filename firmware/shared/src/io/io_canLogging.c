#include "io_canLogging.h"
#include "io_lfs_config.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"
#include "hw_gpio.h"

// Private globals.
static const CanConfig *config;
#define QUEUE_SIZE 2000
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define PATH_LENGTH 10
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];

static uint32_t current_bootcount;

static lfs_t             lfs;
static lfs_file_t        file;
static struct lfs_config cfg;

extern SdCard sd;
extern Gpio   sd_present;
extern bool   sd_inited;

static char current_path[10];

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

#define MAX_NUM_MSGS (IO_LFS_CACHE_SIZE / sizeof(CanMsg))

static char                  file_buffer[IO_LFS_CACHE_SIZE];
const struct lfs_file_config fcfg = {
    .buffer = file_buffer,
};

// assume the lfs is already mounted
static void init_logging_file_system()
{
    // early return
    if (!sd_inited || hw_gpio_readPin(&sd_present))
    {
        return;
    }

    // config the file system
    io_lfs_config(sd.hsd->SdCard.BlockSize, sd.hsd->SdCard.BlockNbr, &cfg);

    uint32_t bootcount = 0;
    // lfs_format(&lfs, &cfg); // test for now
    int err = lfs_mount(&lfs, &cfg);
    if (err)
    {
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }

    // get bootcount value from the file; use this to create new file for logging
    lfs_file_opencfg(&lfs, &file, "bootcount", LFS_O_RDWR | LFS_O_CREAT, &fcfg);
    lfs_file_read(&lfs, &file, &bootcount, sizeof(bootcount));

    // update bootcount for next boot
    bootcount += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &bootcount, sizeof(bootcount));
    current_bootcount = bootcount;
    lfs_file_close(&lfs, &file);

    // create new file on root based on the bootcount
    sprintf((char *)current_path, "%lu", bootcount);

    lfs_file_opencfg(&lfs, &file, current_path, LFS_O_RDWR | LFS_O_CREAT, &fcfg); // this file opens forever
}

void io_canLogging_init(const CanConfig *can_config)
{
    assert(can_config != NULL);
    config = can_config;

    // Initialize CAN queues.

    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);

    // create new folder for this boot
    init_logging_file_system();
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
    if (!sd_inited && hw_gpio_readPin(&sd_present))
    {
        return;
    }

    CanMsg tx_msg;
    osMessageQueueGet(message_queue_id, &tx_msg, NULL, osWaitForever);
    // lfs_file_opencfg(&lfs, &file, current_path, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &fcfg); // this file opens
    // forever
    lfs_ssize_t size = lfs_file_write(&lfs, &file, &tx_msg, sizeof(tx_msg));
    // lfs_file_sync(&lfs, &file);
    // lfs_file_close(&lfs, &file); // this file opens forever
    assert(size = sizeof(tx_msg));

    // static uint32_t msgs_written = 0;
    // msgs_written++;
    // if(msgs_written > MAX_NUM_MSGS/2)
    // {
    //     io_canLogging_sync();
    //     msgs_written = 0;
    // }
}

void io_canLogging_sync(void)
{
    assert(lfs_file_sync(&lfs, &file) == 0);
}

void io_canLogging_msgReceivedCallback(CanMsg *rx_msg)
{
    static uint32_t rx_overflow_count = 0;

    if (config->rx_msg_filter != NULL && !config->rx_msg_filter(rx_msg->std_id))
    {
        // Early return if we don't care about this msg via configured filter func.
        return;
    }

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(message_queue_id, rx_msg, 0, 0) != osOK && config->rx_overflow_callback != NULL)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        // config->rx_overflow_callback(++rx_overflow_count);
    }
}