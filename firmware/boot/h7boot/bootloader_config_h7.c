#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include "hw_flash.h"
#include "hw_hal.h"

#define FLASH_WORD_BYTES (FLASH_NB_32BITWORD_IN_FLASHWORD * sizeof(uint32_t))

uint8_t program_buffer[FLASH_WORD_BYTES];

void bootloader_config_program(uint32_t address, uint64_t data)
{
    uint32_t buffer_idx = address % FLASH_WORD_BYTES;
    memcpy(&program_buffer[buffer_idx], &data, sizeof(data));

    if (buffer_idx + sizeof(uint64_t) == FLASH_WORD_BYTES)
    {
        hw_flash_programFlashWord(address / FLASH_WORD_BYTES * FLASH_WORD_BYTES, (uint32_t *)program_buffer);
    }
}