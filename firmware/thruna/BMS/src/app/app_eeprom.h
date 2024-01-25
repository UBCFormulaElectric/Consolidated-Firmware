#include "App_SharedExitCode.h"
#include "io_eeprom.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define EEPROM_PAGE_SIZE ((uint16_t)16U) // in Bytes
#define DEFAULT_SOC_ADDR 1U

/**
 * Write float values to EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param page the number of the page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, must be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to write to EEPROM
 * @param num_floats number of floats to write
 * @return EepromStatus returns success status for debug
 */
EepromStatus app_eeprom_writeFloats(uint16_t page, uint8_t offset, float* input_data, uint8_t num_floats);

/**
 * Read float values to EEPROM
 * @param page the number of the page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, must be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param ouput_data pointer to array of floats to store data from EEPROM
 * @param num_floats number of floats to read
 * @return EepromStatus returns success status for debug
 */
EepromStatus app_eeprom_readFloats(uint16_t page, uint8_t offset, float* output_data, uint8_t num_floats);

/**
 * Erase page in EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param page the number of the page to set to all zeroes
 * @return EepromStatus returns success status for debug
 */
EepromStatus app_eeprom_pageErase(uint16_t page);

/**
 * Update address and write 4 times to EEPROM
 * @note SHOULD ONLY BE CALLED ONCE EVERY 5ms, DOING SO MORE QUICKLY WILL VIOLATE EEPROM SPECS
 * @param address pointer to the the address to store on the EEPROM
 * @return EepromStatus returns success status for debug
 */
EepromStatus app_eeprom_updateSavedSocAddress(uint16_t* address);

/**
 * Read stored copies of last SOC address and perform error checking
 * @param address address where stored SOC values can be found
 * @return ExitCode returns EXIT_CODE_ERROR in case of data corruption or read error
 */
ExitCode app_eeprom_readSocAddress(uint16_t* address);

/**
 * Write SOC value of cell with lowest voltage to EEPROM (writes 4 identical copies to protect against data corruption)
 * @param min_soc value of current min_soc to write
 * @param address page on EEPROM to store soc
 * @return EepromStatus returns status of write operation
 */
EepromStatus app_eeprom_writeMinSoc(float min_soc, uint16_t address);

/**
 * Read previous SOC value of cell with lowest voltage to EEPROM and perform error checking
 * @param address value of current min_soc to read
 * @param min_soc page on EEPROM to retrieve soc from
 * @return ExitCode returns EXIT_CODE_ERROR in case of data corruption or read error
 */
ExitCode app_eeprom_readMinSoc(uint16_t address, float* min_soc);
