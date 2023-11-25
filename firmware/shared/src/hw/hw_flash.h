#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Write a byte to flash. Must be erased first.
 * @param address Address to write to.
 * @param data Data to write.
 * @return True if successful, otherwise false.
 */
bool hw_flash_programByte(uint32_t address, uint8_t data);

/**
 * Write 2 bytes to flash. Must be erased first.
 * @param address Address to write to.
 * @param data Data to write.
 * @return True if successful, otherwise false.
 */
bool hw_flash_programHalfWord(uint32_t address, uint16_t data);

/**
 * Write 4 bytes to flash. Must be erased first.
 * @param address Address to write to.
 * @param data Data to write.
 * @return True if successful, otherwise false.
 */
bool hw_flash_programWord(uint32_t address, uint32_t data);

/**
 * Write an array bytes to flash. Must be erased first.
 * @param address Address to write to.
 * @param data Pointer to data buffer.
 * @param size Size of data to write, in bytes.
 * @return True if successful, otherwise false.
 */
bool hw_flash_program(uint32_t address, uint8_t *buffer, uint32_t size);

/**
 * Erase a sector of flash. See the microcontroller's reference manual for a list of
 * the available flash sectors and their size. For example, for STM32F412 MCUs see page 59 of:
 * https://www.st.com/resource/en/reference_manual/dm00180369-stm32f412-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
 * @param sector Sector to erase.
 * @return True if successful, otherwise false.
 */
bool hw_flash_eraseSector(uint8_t sector);