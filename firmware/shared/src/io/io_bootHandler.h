#pragma once
#include "io_canMsg.h"
#include "app_utils.h"

NORET void io_bootHandler_processBootRequest(const CanMsg *msg);
