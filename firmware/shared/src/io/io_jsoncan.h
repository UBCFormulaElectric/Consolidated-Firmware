#pragma once

#include "io_can.h"
#include "Io_CanTx.h"

/**
 * Helper functions to interface our hw-level CAN driver (hw_can.h/c)
 * with jsoncan. jsoncan is independent of our firmware by design so it can
 * also run on DIMOS. However, this means we need these functions to translate
 * to/from jsoncan messages.
 */

/**
 * Enqueue a jsoncan msg to be transmitted on the bus.
 * This just invokes io_can_pushTxMsgToQueue, but we need this intermediate
 * function since jsoncan defines its own CAN message struct.
 * @param msg CAN msg to be TXed.
 */
void io_jsoncan_pushTxMsgToQueue(const JsonCanMsg *msg);

/**
 * Convert a jsoncan message to our own driver-level CAN message type.
 * @param src Pointer to jsoncan message to be copied from.
 * @param dest Pointer to CAN message to be copied to.
 */
void io_jsoncan_copyToCanMsg(const JsonCanMsg *src, CanMsg *dest);

/**
 * Convert own driver-level CAN message type to a jsoncan message.
 * @param src Pointer to CAN message to be copied from.
 * @param dest Pointer to jsoncan message to be copied to.
 */
void io_jsoncan_copyFromCanMsg(const CanMsg *src, JsonCanMsg *dest);
