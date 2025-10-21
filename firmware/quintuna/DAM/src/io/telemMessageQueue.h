#pragma once

// the telemMessageQueue exclusively processes the broadcasting of canmsgs

#include <stdbool.h>
#include "io/canMsg.h"

void   io_telemMessageQueue_init(void);
bool   io_telemMessageQueue_pushTx(const CanMsg *rx_msg);
CanMsg io_telemMessageQueue_popTx(void);