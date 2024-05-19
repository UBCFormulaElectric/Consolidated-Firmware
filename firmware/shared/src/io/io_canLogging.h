#include "io_can.h"
#include "hw_sd.h"
#include "lfs.h"

typedef struct
{
    uint32_t id : 11;
    uint32_t dlc : 4;        // Data length code [0,8]
    uint32_t timestamp : 17; // sum up to 32 bits
    uint8_t  data[8];
    uint8_t  reserved[4]; // reserved byte to round up to 16 bytes
} CanMsgLog;

/**
 * Create a new message queue for can logging message
 * Create a new file for this boot for record message
 * @param can_config
 *
 */
int io_canLogging_init(const CanConfig *can_config);

/**
 * Create a push message to
 */
int io_canLogging_pushTxMsgToQueue(const CanMsg *msg);

/**
 * write the can message to the sdcard
 * pop the massage from queue
 *
 */
int io_canLogging_recordMsgFromQueue(void);

void io_canLogging_loggingQueuePush(CanMsg *rx_msg);

int io_canLogging_sync(void);
