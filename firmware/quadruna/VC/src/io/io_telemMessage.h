#pragma once
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"
#include "hw_can.h"

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
 * Serializes the can msg and sends it over UART to the appropriate Modem
 */

bool io_telemMessage_broadcast(CanMsg *rx_msg);