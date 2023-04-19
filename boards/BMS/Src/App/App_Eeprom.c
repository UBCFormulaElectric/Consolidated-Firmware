#include "App_Eeprom.h"
#include "Io_Eeprom.h"

#define SOC_ADDR_PAGE (0U)
#define SOC_ADDR_OFFSET (0U)
#define SOC_ADDR_SIZE_BYTES (1U)

#define NUM_PAGES (128U)

uint8_t App_Eeprom_InitializeSocAddr(void)
{
    // retrieve address where SOC can be found
    uint8_t soc_page;
    Io_Eeprom_ReadPage(SOC_ADDR_PAGE, SOC_ADDR_OFFSET, &soc_page, SOC_ADDR_SIZE_BYTES);

    // read last saved soc value from EEPROM
    uint8_t soc_data[PAGE_SIZE];
    Io_Eeprom_ReadPage(soc_page, SOC_ADDR_OFFSET, soc_data, PAGE_SIZE);

    // re-write last soc to incremented address

    soc_page++;

    if (soc_page > NUM_PAGES)
    {
        soc_page = 1U;
    }

    Io_Eeprom_WritePage(soc_page, SOC_ADDR_OFFSET, soc_data, PAGE_SIZE);

    // return new address
    return soc_page;
}

void App_Eeprom_SaveActiveSocAddr(uint8_t soc_addr)
{
    // write new active address to page 0
    Io_Eeprom_WritePage(SOC_ADDR_PAGE, SOC_ADDR_OFFSET, &soc_addr, SOC_ADDR_SIZE_BYTES);
}

uint8_t App_Eeprom_ResetSocAddr(void)
{
    // reset contents of SOC address page
    uint8_t soc_addr = 1U;
    Io_Eeprom_WritePage(SOC_ADDR_PAGE, SOC_ADDR_OFFSET, &soc_addr, SOC_ADDR_SIZE_BYTES);

    return soc_addr;
}
