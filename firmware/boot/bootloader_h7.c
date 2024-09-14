#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include "hw_flash.h"
#include "hw_hal.h"

#define FLASH_WORD_BYTES (FLASH_NB_32BITWORD_IN_FLASHWORD * sizeof(uint32_t))

uint8_t program_buffer[FLASH_WORD_BYTES];

void bootloader_boardSpecific_program(uint32_t address, uint64_t data)
{
    uint32_t buffer_idx = address % FLASH_WORD_BYTES;
    memcpy(&program_buffer[buffer_idx], &data, sizeof(data));

    // On the STM32H733xx microcontroller, the smallest amount of memory you can
    // program at a time is 32 bytes (one "flash word"). This means we cannot
    // program the data from a single CAN message, since the CAN message has at most 8 bytes of data.
    // So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then
    // we can write the entire flash word. This implementation only works if the binary size
    // is a multiple of 32 bytes, or the buffer won't fill up for the last few bytes so won't be written
    // into flash. This is guaranteed by canup.
    if (buffer_idx + sizeof(uint64_t) == FLASH_WORD_BYTES)
    {
        hw_flash_programFlashWord(address / FLASH_WORD_BYTES * FLASH_WORD_BYTES, (uint32_t *)program_buffer);
    }
}

void bootloader_boardSpecific_program_length(uint32_t address, uint8_t *data, uint8_t length)
{
    // replacement for the global variable, keep the origial for reference
    const uint32_t FLASH_WORD_BYTES_7 = FLASH_WORD_BYTES * 7;
    static uint8_t buffer[FLASH_WORD_BYTES];


    uint32_t       buffer_idx = address % (FLASH_WORD_BYTES);
    memcpy(&buffer[buffer_idx], data, length);

    // On the STM32H733xx microcontroller, the smallest amount of memory you can
    // program at a time is 32 bytes (one "flash word"). This means we cannot
    // program the data from a single CAN message, since the CAN message has at most 8 bytes of data.
    // So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then
    // we can write the entire flash word. This implementation only works if the binary size
    // is a multiple of 32 bytes, or the buffer won't fill up for the last few bytes so won't be written
    // into flash. This is guaranteed by canup.
    if (buffer_idx + length == (FLASH_WORD_BYTES))
    {
        hw_flash_programFlashWord(address / FLASH_WORD_BYTES * FLASH_WORD_BYTES, (uint32_t *)buffer);
    }
}