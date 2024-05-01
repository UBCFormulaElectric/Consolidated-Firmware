#pragma once
#include "hw_uart.h"
/**
 * Creates a new message queue for storing telem messages
 *
 */
void io_telemMessage_init();

/**
 * Serializes the can msg and sends it over UART to the appropriate Modem
 */

void io_telemMessage_broadcast(UART *modem);