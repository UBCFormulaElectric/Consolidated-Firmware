/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "io_canTx.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Returns true if JCT receives the specified message ID.
 */
bool io_canRx_filterMessageId(uint32_t std_id);

/**
 * Unpack a received message and update the CAN RX table.
 */
void io_canRx_updateRxTableWithMessage(JsonCanMsg* msg);

