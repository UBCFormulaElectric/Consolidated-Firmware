#pragma once

#include "io_canRx.h"
#include "io_canMsg.h"

/**
 * Helper functions to interface our hw-level CAN driver (hw_can.h/c)
 * with jsoncan. jsoncan is independent of our firmware by design so it can
 * also run on DIMOS. However, this means we need these functions to translate
 * to/from jsoncan messages.
 */

/**
 * Convert own driver-level CAN message type to a jsoncan message.
 * @param msg message to convert
 */
JsonCanMsg io_jsoncan_copyFromCanMsg(const CanMsg *msg);

/**
 * Convert a jsoncan message to own driver-level CAN message type.
 * @param msg message to convert
 */
CanMsg io_jsoncan_copyToCanMsg(const JsonCanMsg *msg);
