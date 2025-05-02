#include "hw_spis.h"
#include "hw_gpios.h"
#include "main.h"

#include <assert.h>

static SpiBus isospi_bus = { .handle = &hspi2 };

#define LTC6813_SPI_TIMEOUT_MS (10U)
const SpiDevice ltc6813_spi = { .bus = &isospi_bus, .nss_pin = &spi_cs_pin, .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi2);
    return &isospi_bus;
}
