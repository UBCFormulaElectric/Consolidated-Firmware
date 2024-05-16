#pragma once
#include "app_utils.h"
#include "io_can.h"

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"

typedef struct
{
    UART *modem900M;
    UART *modem2_4G;
} Modem;
#else
EMPTY_STRUCT(Modem);
#endif

/**
 * Creates a new message queue for storing telem messages
 *
 */
void io_telemMessage_init(Modem *m);

/**
 * Serializes the can msg and pushes it to the queue
 */
bool io_telemMessage_pushMsgtoQueue(CanMsg *rx_msg);

/**
 * Pops message from the queue and passes over uart
 */
bool io_telemMessage_broadcastMsgFromQueue(void);