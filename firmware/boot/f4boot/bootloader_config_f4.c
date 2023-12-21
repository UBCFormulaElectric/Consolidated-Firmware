#include "bootloader.h"
#include "hw_flash.h"
#include "hw_hal.h"

void bootloader_config_program(uint32_t address, uint64_t data)
{
    uint32_t bottom_word = (uint32_t)data & 0xFFFFFFFFU;
    uint32_t top_word    = (uint32_t)(data >> 32) & 0xFFFFFFFFU;
    hw_flash_programWord(address, bottom_word);
    hw_flash_programWord(address + sizeof(uint32_t), top_word);
}