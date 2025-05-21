#pragma once
#include "io_canMsg.h"
#include <stdbool.h>
#include "io_rtc.h"

/**
 * Creates a new message queue for storing telem messages
 */
void io_telemMessage_init();

/**
 * Serializes the can msg and pushes it to the queue
 */
bool io_telemMessage_pushMsgtoQueue(const CanMsg *rx_msg);

/**
 * Pops message from the queue and passes over uart
 */
bool io_telemMessage_broadcastMsgFromQueue(void);