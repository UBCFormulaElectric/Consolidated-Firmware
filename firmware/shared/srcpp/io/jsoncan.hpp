#pragma once

#include "io/canMsgQueue.hpp"

extern "C"
{
#include "io_canTx.h"
}

/**
 * Helper functions to interface our hw-level CAN driver (hw_can.h/c)
 * with jsoncan. jsoncan is independent of our firmware by design so it can
 * also run on DIMOS. However, this means we need these functions to translate
 * to/from jsoncan messages.
 */

namespace io::jsoncan
{
/**
 * Convert a jsoncan message to our own driver-level CAN message type.
 * @param src Pointer to jsoncan message to be copied from.
 * @param dest Pointer to CAN message to be copied to.
 */
void copyToCanMsg(const JsonCanMsg *src, hw::can::CanMsg *dest);

/**
 * Convert own driver-level CAN message type to a jsoncan message.
 * @param src Pointer to CAN message to be copied from.
 * @param dest Pointer to jsoncan message to be copied to.
 */
void copyFromCanMsg(const hw::can::CanMsg *src, JsonCanMsg *dest);
} // namespace io::jsoncan
