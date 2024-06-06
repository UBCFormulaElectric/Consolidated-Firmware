#include "bootloader.h"
#include "hw_flash.h"
#include "hw_hal.h"

void bootloader_boardSpecific_program(uint32_t address, uint32_t *cache_buffer)
{
    for (uint32_t i = 0; i < CHUNK_SIZE_BYTES / 4 - 1; i += 2)
    {
        hw_flash_programWord(address, cache_buffer[i]);
        hw_flash_programWord(address + sizeof(uint32_t), cache_buffer[i + 1]);
        address += sizeof(uint64_t);
    }
}
