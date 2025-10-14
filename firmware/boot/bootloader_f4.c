#include "bootloader.h"
#include "hw_flash.h"
#include "hw_can.h"
#include <assert.h>

extern CanHandle can;
const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}

void bootloader_boardSpecific_program(uint32_t address, uint64_t data)
{
    uint32_t bottom_word = (uint32_t)data & 0xFFFFFFFFU;
    uint32_t top_word    = (uint32_t)(data >> 32) & 0xFFFFFFFFU;
    hw_flash_programWord(address, bottom_word);
    hw_flash_programWord(address + sizeof(uint32_t), top_word);
}
