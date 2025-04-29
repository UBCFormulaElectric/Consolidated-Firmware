#pragma once
#include <stdint.h>
#include <stdbool.h>

// TODO when jiajun gets can routing done
// #include <io_canRouting>

#ifdef STM32F412Rx
#define CAN_PAYLOAD_BYTES 8
#elif STM32H733xx
#define CAN_PAYLOAD_BYTES 64
#else
#error "Please define what MCU is used."
#endif

typedef struct
{
    uint32_t std_id;
    // classic data length range : [0, 8]
    // fd data length range : [0, 64]
    uint32_t dlc;
    uint32_t timestamp;
    uint8_t  data[CAN_PAYLOAD_BYTES];
    uint8_t  bus; // TODO change this to the bus` enum when complete
    bool     is_fd;
} CanMsg;