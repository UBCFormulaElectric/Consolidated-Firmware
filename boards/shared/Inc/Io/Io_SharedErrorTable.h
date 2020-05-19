#pragma once

#include "Io_SharedCanMsg.h"

void Io_SharedErrorTable_SetErrorsBasedOnCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg);
