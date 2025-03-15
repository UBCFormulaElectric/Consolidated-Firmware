#pragma once
#include "io_canMsg.h"
#include <stdbool.h>

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

/**
 * Appends the header to the frame buffer
 */
bool telemMessage_appendHeader(uint8_t *frame_buffer, uint8_t *proto_buffer, uint8_t proto_msg_length);

/**
 * Builds the frame from the received message
 */
bool telemMessage_buildFrameFromRxMsg(const CanMsg *rx_msg, uint8_t *frame_buffer, uint8_t *frame_length);