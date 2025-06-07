#pragma once

#include "io_canMsg.h"
void io_bootloadeReroute_init(
    void (*transmit_can2_msg_func)(const CanMsg *));
    
void io_bootloaderReroute_reRoute(const CanMsg *msg);
