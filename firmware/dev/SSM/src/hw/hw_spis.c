#include "hw_spis.h"

#define ISOSPI_TIMEOUT_MS (10U)
#define DAC_TIMEOUT_MS (10U)

static SpiBus   dac_bus = { .handle = &hspi1 };
const SpiDevice dac     = { .bus = &dac_bus, .nss_pin = &dac_chipSelect, .timeout_ms = DAC_TIMEOUT_MS };

static SpiBus   isospi_bus = { .handle = &hspi2 };
const SpiDevice isospi_ls  = { .bus = &isospi_bus, .nss_pin = &isospi_chipSelect_ls, .timeout_ms = ISOSPI_TIMEOUT_MS };
const SpiDevice isospi_hs  = { .bus = &isospi_bus, .nss_pin = &isospi_chipSelect_hs, .timeout_ms = ISOSPI_TIMEOUT_MS };

// Conversion from SPI HAL handles to the respective bus struct.
SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi1)
        return &dac_bus;
    if (handle == &hspi2)
        return &isospi_bus;
    return NULL;
}
