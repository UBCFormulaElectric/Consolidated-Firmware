#pragma once
#include "io_canMsg.h"

#ifndef BOOTLOADER
#error "This header should only be included in the bootloader build"
#endif

void io_bootHandler_processBootRequest(const CanMsg *msg);
