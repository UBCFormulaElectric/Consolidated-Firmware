#include "io_can.h"
#include "hw_sd.h"
#include "lfs.h"
/**
 * Create a new message queue for can logging message
 * Create a new file for this boot for record message
 * @param can_config
 *
 */
void io_canLogging_init(const CanConfig *can_config, struct lfs_config *cfg);

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

void io_canLogging_msgReceivedCallback(uint32_t rx_fifo, CanMsg* rx_msg);