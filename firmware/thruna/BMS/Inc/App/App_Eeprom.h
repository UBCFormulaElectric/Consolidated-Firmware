#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "App_SharedExitCode.h"
#include "App_EepromExitCode.h"

#define PAGE_SIZE ((uint16_t)16U) // in Bytes
#define DEFAULT_SOC_ADDR 1U

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
EEPROM_StatusTypeDef App_Eeprom_UpdateSavedAddress(struct Eeprom *eeprom, uint16_t current_address);

/**
 * Read stored copies of last SOC address and perform error checking
 * @param eeprom Eeprom to erase a page from
 * @param address address where stored SOC values can be found
 * @return ExitCode returns EXIT_CODE_ERROR in case of data corruption or read error
 */
ExitCode App_Eeprom_ReadAddress(struct Eeprom *eeprom, uint16_t *address);

/**
 * Write SOC value of cell with lowest voltage to EEPROM (writes 4 identical copies to protect against data corruption)
 * @param eeprom EEPROM to write SOC to
 * @param min_soc value of current min_soc to write
 * @param address page on EEPROM to store soc
 * @return EEPROM_StatusTypeDef returns status of write operation
 */
EEPROM_StatusTypeDef App_Eeprom_WriteMinSoc(struct Eeprom *eeprom, float min_soc, uint16_t address);

/**
 * Read previous SOC value of cell with lowest voltage to EEPROM and perform error checking
 * @param eeprom
 * @param address
 * @param min_soc
 * @return ExitCode returns EXIT_CODE_ERROR in case of data corruption or read error
 */
ExitCode App_Eeprom_ReadMinSoc(struct Eeprom *eeprom, uint16_t address, float *min_soc);
