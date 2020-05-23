#pragma once

#include "Io_SharedCanMsg.h"

void Io_SharedErrorTable_SetErrorsFromCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg);
