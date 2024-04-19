#pragma once
#include "hw_can.h"

/**
 * Creates a new message queue for storing telem messages
 *
 */
void io_telemMessage_init();

/**
 * Takes a can message, determines if we 'want' it and then serializes it and then pushes it to the queue
 */

void io_telemMessage_pushMsgToQueue(const CanMsg *msg);

/**
 * Pops a message from the telemMessage que and sends it over UART to the appropriate Modem
 */

void io_telemMessage_popMsgFromQueue();