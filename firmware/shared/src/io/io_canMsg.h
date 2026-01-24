#pragma once
#include <stdint.h>
#include <stdbool.h>

#if defined(STM32F412Rx)
#define CAN_PAYLOAD_BYTES 8
#elif defined(STM32H733xx) || defined(STM32H562xx)
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
    union
    {
        uint8_t  data8[CAN_PAYLOAD_BYTES];
        uint16_t data16[CAN_PAYLOAD_BYTES / 2];
        uint32_t data32[CAN_PAYLOAD_BYTES / 4];
        uint64_t data64[CAN_PAYLOAD_BYTES / 8];
    } data;
    uint8_t bus; // TODO change this to the bus` enum when complete
    bool    is_fd;
} CanMsg;
