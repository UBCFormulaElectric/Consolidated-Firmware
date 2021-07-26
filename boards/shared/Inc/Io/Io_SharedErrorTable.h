#pragma once

#include "Io_SharedCanMsg.h"

/**
 * Update error table based on incoming CAN message
 * @param error_table The error table to write to
 * @param can_msg The CAN message being received
 */
void Io_SharedErrorTable_SetErrorsFromCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg);

/**
 * Update error table based on outgoing CAN message being sent by the BMS
 * @param error_table The error table to write to
 * @param can_msg The CAN message being broadcast by the BMS
 */
void Io_SharedErrorTable_SetErrorsFromBmsCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg);
