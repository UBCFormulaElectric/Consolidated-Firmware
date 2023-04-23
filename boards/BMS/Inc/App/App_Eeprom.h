#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define PAGE_SIZE ((uint16_t)16U) // in Bytes

struct Eeprom;

/**
 * Allocate and initialize a EEPROM
 * @param write_page A function that can be called to get write to a page on the eeprom
 * @param read_page A function that can be called to get read from a page on the eeprom
 * @param page_erase A function that can be called to erase a page on the eeprom
 * system voltage
 */
struct Eeprom *App_Eeprom_Create(
    uint8_t (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    uint8_t (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    uint8_t (*page_erase)(uint16_t));

/**
 * Deallocate the memory used by the given Eeprom
 * @param eeprom The EEPROM status to deallocate
 */
void App_Eeprom_Destroy(struct Eeprom *eeprom);

/**
 * Write float values to EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, should ideally be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to write to EEPROM
 * @param num_floats number of floats to write
 * @return uint8_t returns success status for debug
 */
uint8_t
    App_Eeprom_WriteFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats);

/**
 * Read float values to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, should ideally be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to read from EEPROM
 * @param num_floats number of floats to read
 * @return uint8_t returns success status for debug
 */
uint8_t
    App_Eeprom_ReadFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats);
