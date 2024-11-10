#include "io_can.h"

#pragma once

typedef struct
{
    uint32_t checksum;
    uint32_t size_bytes;
    utin32_t bootloader_status;
} Metadata;


void io_bootloader_checkBootMsg(CanMsg *msg);