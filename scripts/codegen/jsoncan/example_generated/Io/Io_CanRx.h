/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "Io_SharedCanMsg.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Returns true if JCT receives the specified message ID.
 */
bool Io_CanRx_FilterMessageId(uint32_t std_id);

/**
 * Unpack a received message and update the CAN RX table.
 */
void Io_CanRx_UpdateRxTableWithMessage(struct CanMsg* msg);

