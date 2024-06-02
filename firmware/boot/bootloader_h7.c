#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include "hw_flash.h"
#include "hw_hal.h"

#define FLASH_WORD_BYTES (FLASH_NB_32BITWORD_IN_FLASHWORD * sizeof(uint32_t))

void bootloader_boardSpecific_program(uint32_t address, uint64_t* cache_buffer)
{
    // On the STM32H733xx microcontroller, the smallest amount of memory you can
    // program at a time is 32 bytes (one "flash word"). This means we cannot
    // program the data from a single CAN message, since the CAN message has at most 8 bytes of data.
    // So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then
    // we can write the entire flash word. This implementation only works if the binary size
    // is a multiple of 32 bytes, or the buffer won't fill up for the last few bytes so won't be written
    // into flash. This is guaranteed by canup.

    uint8_t flash_buffer[FLASH_WORD_BYTES];
    uint32_t flash_buffer_idx = 0;
    uint32_t cache_buffer_idx = 0;

    while (cache_buffer_idx < CHUNK_SIZE_BYTES) {
        if (flash_buffer_idx == FLASH_WORD_BYTES) {
            hw_flash_programFlashWord(address, (uint32_t *)flash_buffer);
            address += FLASH_WORD_BYTES;
            memset(flash_buffer, 0, sizeof(flash_buffer));
            flash_buffer_idx = 0;
        }

        memcpy(&flash_buffer[flash_buffer_idx], &cache_buffer[cache_buffer_idx], sizeof(cache_buffer[cache_buffer_idx]));
        flash_buffer_idx += sizeof(uint64_t);;
        cache_buffer_idx++;
    }
}
