#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cmsis_os.h"
#include "queue.h"
#include "hw_gpio.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
// Private globals.
static const CanConfig *config = NULL;
#define QUEUE_SIZE 2048
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define PATH_LENGTH 10
static osMessageQueueId_t message_queue_id;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];

static uint32_t current_bootcount;
static int      log_fd; // fd for the log file

static char current_path[10];

static const osMessageQueueAttr_t queue_attr = {
    .name      = "CAN Logging Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

// assume the lfs is already mounted
static void initLoggingFileSystem()
{
    // early return
    current_bootcount  = io_fileSystem_getBootCount();

    // create new folder for this boot
    sprintf(current_path, "%lu", current_bootcount);
    log_fd = io_fileSystem_open(current_path);
}

void io_canLogging_init(const CanConfig *can_config)
{
    assert(can_config != NULL);
    config = can_config;

    // Initialize CAN queues.

    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);

    // create new folder for this boot
    initLoggingFileSystem();
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

    io_fileSystem_write(log_fd, &tx_msg, sizeof(tx_msg));
    // assert(size = sizeof(tx_msg));
}

void io_canLogging_msgReceivedCallback(CanMsg *rx_msg)
{
    static uint32_t rx_overflow_count = 0;
    if (config == NULL)
    {
        return;
    }

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

void io_canLogging_sync()
{
    // SAVe the seek before close
    io_fileSystem_sync(log_fd);
}