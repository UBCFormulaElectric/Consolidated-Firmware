#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "hw_flash.hpp"
#include "hw_hal.h"
#include "hw_fdcan.h"

extern CanHandle can;
const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can.hcan);
    return &can;
}

uint8_t program_buffer[WORD_BYTES];

void bootloader_boardSpecific_program(uint32_t address, uint64_t data)
{
    uint32_t buffer_idx = address % WORD_BYTES;
    memcpy(&program_buffer[buffer_idx], &data, sizeof(data));

    // On the STM32H733xx microcontroller, the smallest amount of memory you can program at a
    // time is 32 bytes (one "flash word"). On the STM32H562xx microcontroller, the largest amount of
    // memory you can at a time is 16 bytes (one "quadword"). This means we cannot
    // program the data from a single CAN message, since the CAN message has at most 8 bytes of data.
    // So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then
    // we can write the entire flash word. This implementation only works if the binary size
    // is a multiple of 32 bytes, or the buffer won't fill up for the last few bytes so won't be written
    // into flash. This is guaranteed by canup.
    if (buffer_idx + sizeof(uint64_t) == WORD_BYTES)
    {
        programBuffer(address / FLASH_WORD_BYTES * FLASH_WORD_BYTES, (uint32_t *)program_buffer);
    }
}
