#pragma once
#include <stdint.h>

// TODO when jiajun gets can routing done
// #include <io_canRouting>

#define CAN_PAYLOAD_BYTES 8

typedef struct
{
    uint32_t std_id;
    uint32_t dlc; // data length range : [0, 8]
    uint32_t timestamp;
    uint8_t  data[CAN_PAYLOAD_BYTES];
    uint8_t  bus; // TODO change this to the bus enum when complete
} CanMsg;
