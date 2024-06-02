#include "bootloader.h"
#include "hw_flash.h"
#include "hw_hal.h"

void bootloader_boardSpecific_program(uint32_t address, uint64_t *cache_buffer)
{
    for (uint64_t i = 0; i < CHUNK_SIZE_BYTES / 8; i++)
    {
        uint32_t bottom_word = (uint32_t)cache_buffer[i] & 0xFFFFFFFFU;
        uint32_t top_word    = (uint32_t)(cache_buffer[i] >> 32) & 0xFFFFFFFFU;
        hw_flash_programWord(address, bottom_word);
        hw_flash_programWord(address + sizeof(uint32_t), top_word);
    }
}