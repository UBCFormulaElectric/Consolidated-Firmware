#include "io_can.h"
#include "hw_sd.h"
#include "lfs.h"

typedef struct
{
    uint32_t id : 11;
    uint32_t dlc : 4;        // Data length code [0, 8)
    uint32_t timestamp : 17; // sum up to 32 bits
    uint8_t  data[8];
} CanMsgLog;

/**
 * Create a new message queue for can logging message
 * Create a new file for this boot for record message
 * @param can_config
 *
 */
void io_canLogging_init(const CanConfig *can_config);

/**
 * Create a push message to
 */
void io_canLogging_pushTxMsgToQueue(const CanMsg *msg);

/**
 * write the can message to the sdcard
 * pop the massage from queue
 *
 */
void io_canLogging_recordMsgFromQueue(void);

void io_canLogging_msgReceivedCallback(CanMsg *rx_msg);

void io_canLogging_sync(void);