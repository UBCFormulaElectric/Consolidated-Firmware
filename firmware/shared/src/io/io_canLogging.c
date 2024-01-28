#include "io_canLogging.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"
#include "hw_gpio.h"

// Private globals.
static const CanConfig *config;
#define QUEUE_SIZE 20
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define PATH_LENGTH 10
static osMessageQueueId_t message_queue_id;
static StaticQueue_t queue_control_block;
static uint8_t queue_buf[QUEUE_BYTES];

#define QUEUE_SIZE 20
static uint32_t current_bootcount;

static lfs_t lfs;
static lfs_file_t file;

extern Gpio sd_enable;
extern bool sd_inited;

static char current_path[10];

static const osMessageQueueAttr_t queue_attr = {
    .name = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem = &queue_control_block,
    .cb_size = sizeof(StaticQueue_t),
    .mq_mem = queue_buf,
    .mq_size = QUEUE_BYTES,
};

static char buffer[512];
const struct lfs_file_config fcfg = {
    .buffer = buffer,
};

static void generateCANMsg(void)
{
    lfs_file_opencfg(&lfs, &file, "canMsg", LFS_O_RDWR | LFS_O_CREAT, &fcfg);
    CanMsg msg;
    uint32_t i = 0;
    msg.dlc = 1;

    // write 0 to 99
    for (i = 0; i < 100; i++)
    {
        msg.std_id = i;
        msg.data[0] = (uint8_t)i;
        lfs_file_write(&lfs, &file, &msg, sizeof(msg));
    }

    msg.dlc = 5;
    memcpy(&msg.data, "hello", 5);
    // write hello
    for (i = 0; i < 100; i++)
    {
        msg.std_id = i + 100;
        lfs_file_write(&lfs, &file, &msg, sizeof(msg));
    }
    lfs_file_close(&lfs, &file);
}

// assume the lfs is already mounted
static void createFolder(struct lfs_config *cfg)
{
    // early return
    if (!sd_inited && hw_gpio_readPin(&sd_enable))
    {
        return;
    }

    uint32_t bootcount = 0;
    int err = lfs_mount(&lfs, cfg);
    if (err)
    {
        lfs_format(&lfs, cfg);
        err = lfs_mount(&lfs, cfg);
    }

    lfs_file_opencfg(&lfs, &file, "bootcount", LFS_O_RDWR | LFS_O_CREAT, &fcfg);
    lfs_file_read(&lfs, &file, &bootcount, sizeof(bootcount));

    // update bootcount
    bootcount += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &bootcount, sizeof(bootcount));
    current_bootcount = bootcount;

    // close the bootcount file
    lfs_file_close(&lfs, &file);
    generateCANMsg();

    // create new folder on root based on the bootcount

    sprintf((char *)current_path, "%lu", bootcount);

    // open a file for logging can message
    lfs_file_opencfg(&lfs, &file, current_path, LFS_O_RDWR | LFS_O_CREAT, &fcfg);
    CanMsg msg = {
        .dlc = 12,
        .std_id = 12,
    };
    msg.data[0] = 0xff;
    lfs_file_write(&lfs, &file, &msg, sizeof(msg));
    lfs_file_close(&lfs, &file);
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
    if (!sd_inited && hw_gpio_readPin(&sd_enable))
    {
        return;
    }
    CanMsg tx_msg;
    osMessageQueueGet(message_queue_id, &tx_msg, NULL, osWaitForever);
    lfs_file_opencfg(&lfs, &file, current_path, LFS_O_RDWR | LFS_O_CREAT, &fcfg);
    lfs_file_seek(&lfs, &file, 0, SEEK_END);
    lfs_ssize_t size = lfs_file_write(&lfs, &file, &tx_msg, sizeof(tx_msg));
    lfs_file_close(&lfs, &file);
    if (size)
    {
    }
    // write the message to the file system
}

void io_canLogging_msgReceivedCallback(uint32_t rx_fifo, CanMsg *rx_msg)
{
    static uint32_t rx_overflow_count = 0;

    // if (config->rx_msg_filter != NULL && !config->rx_msg_filter(rx_msg->std_id))
    // {
    //     // Early return if we don't care about this msg via configured filter func.
    //     return;
    // }

    // We defer reading the CAN RX message to another task by storing the
    // message on the CAN RX queue.
    if (osMessageQueuePut(message_queue_id, rx_msg, 0, 0) != osOK && config->rx_overflow_callback != NULL)
    {
        // If pushing to the queue failed, the queue is full. Discard the msg and invoke the RX overflow callback.
        // config->rx_overflow_callback(++rx_overflow_count);
    }
}