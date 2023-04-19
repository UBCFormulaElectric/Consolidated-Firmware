#include <stdint.h>

/**
 * Increment return the page where the SOC can currently be found, used for wear-leveling reasons.
 * @param none
 * @return uint8_t page number for active SOC data storage
 */
uint8_t App_Eeprom_InitializeSocAddr(void);

/**
 * Save the newly incremented SOC page to page 0
 * @param uint8_t Page number for active SOC data storage
 * @return none
 */
void App_Eeprom_SaveActiveSocAddr(uint8_t soc_addr);

/**
 * reset the active SOC page to page 1
 * @param none
 * @return uint8_t page number for active SOC data storage
 */
uint8_t App_Eeprom_ResetSocAddr(void);
