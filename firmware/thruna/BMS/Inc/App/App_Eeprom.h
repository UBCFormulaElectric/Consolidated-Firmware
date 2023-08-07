#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "App_EepromExitCode.h"

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
    EEPROM_StatusTypeDef (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    EEPROM_StatusTypeDef (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    EEPROM_StatusTypeDef (*page_erase)(uint16_t));

/**
 * Deallocate the memory used by the given Eeprom
 * @param eeprom The EEPROM to deallocate
 */
void App_Eeprom_Destroy(struct Eeprom *eeprom);

/**
 * Write float values to EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param eeprom Eeprom to write to
 * @param page the number of the page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, must be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to write to EEPROM
 * @param num_floats number of floats to write
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef
    App_Eeprom_WriteFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats);

/**
 * Read float values to EEPROM
 * @param eeprom Eeprom to read from
 * @param page the number of the page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, must be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param ouput_data pointer to array of floats to store data from EEPROM
 * @param num_floats number of floats to read
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef
    App_Eeprom_ReadFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats);

/**
 * Erase page in EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param eeprom Eeprom to erase a page from
 * @param page the number of the page to set to all zeroes
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef App_Eeprom_PageErase(struct Eeprom *eeprom, uint16_t page);

/**
 * Write an address 4 times to EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param eeprom Eeprom to write address to
 * @param page the number of the page to write to
 * @param address the address to store on the EEPROM
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef App_Eeprom_Write4CopiesOfAddress(struct Eeprom *eeprom, uint16_t page, uint16_t address);

/**
 * Read the stored 4 copies of adress from EEPROM
 * @param eeprom Eeprom to erase a page from
 * @param page the number of the page to read from
 * @param addresses array of 4 addresses to be chosen from. Should all contain same value but may not due to
 * data-corruption in EEPROM
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef App_Eeprom_Read4CopiesOfAddress(struct Eeprom *eeprom, uint16_t page, uint16_t *addresses);
