#pragma once

#include <stdint.h>

struct CanMsg
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t  data[8];
};
