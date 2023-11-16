#pragma once

#include <stdint.h>
#include <stdbool.h>

// Programming double word doesn't work for some magical reason...

bool hw_flash_programByte(uint32_t address, uint8_t data);
bool hw_flash_programHalfWord(uint32_t address, uint16_t data);
bool hw_flash_programWord(uint32_t address, uint32_t data);
bool hw_flash_program(uint32_t address, uint8_t *buffer, uint32_t size);
bool hw_flash_eraseSector(uint8_t sector);