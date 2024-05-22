#pragma once
#include "app_utils.h"
#include "io_can.h"

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"

typedef struct
{
    UART const *modem900M;
    UART const *modem2_4G;
} Modem;
#else
EMPTY_STRUCT(Modem);
#endif

/**
 * Creates a new message queue for storing telem messages
 *
 */
void io_telemMessage_init(const Modem *m);

/**
 * Serializes the can msg and sends it over UART to the appropriate Modem
 */

bool io_telemMessage_broadcast(CanMsg *rx_msg);
