#pragma once

#include "Io_SharedCanMsg.h"
#include "App_SharedErrorTable.h"

/**
 * Update error table based on incoming CAN message
 * @param error_table The error table to write to
 * @param can_msg The CAN message being received
 */
void Io_SharedErrorTable_SetErrorsFromCanMsg(struct ErrorTable *error_table, struct CanMsg *can_msg);
